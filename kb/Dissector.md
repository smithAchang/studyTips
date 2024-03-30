# 前言
`Wireshark`提供了lua扩展能力，可以定制一些`Listner`和`Dissector`，用于一些自定义的使用`Wireshark`场景，例如，
lua插件|适应场景
---------|-----------
Listner|报文统计、内容抽取等
Dissector|协议解析

已在前面的文档中积累了对于`Listener`的使用，见[Listener](https://blog.csdn.net/jkler_doyourself/article/details/76711682)。

本文将继续介绍下`Dissector`的扩展使用，采用对于官网例子比较详细的注释解读方式。

这次尝试源于想固化一次对于接口文档的阅读，以利于在后期可以避免一遍遍翻阅文档，重新查看协议说明和字段字段。

为什么要重新费心费力去做某些重复的事情呢？ 那么利用`Wireshark Lua`脚本将其固化下来，将是非常好的实践！

# 解析器基本组件
组件|说明
---------|-----
Proto| 定义协议，包括字段和解析器函数定义；在解析树中体现为一个节点
ProtoField|定义字段类型
Dissector Chain |解析器链，某个解析器完成自身协议层解析后，在尾部继续调用其它解析器，完成整个解析

## Tvb参数特殊说明

tvb 参数|说明
---------|-----
offset | 必须，且从零开始
length|可选，如果不指定，将获取整个`tvb`长度


tvb API|说明
---------|-----
buf(...):tvb | 调用其它解析器需要重新生成新的tvb


## 启用说明
需要在`Wireshark`安装目录的`init.lua`中增加相应的扩展插件
```lua
dofile(DATA_DIR.."example.lua")
```

# 解析器例子
```lua

-- 分析器根协议
--- + 第一个参数为协议缩写，将后期用于过滤条件，所以，要求全局唯一
--- + 第二个参数为协议描述，默认用于协议解析树的节点Label，要求全局唯一

local p_multi = Proto("multi", "MultiProto")

-- 嵌套自定义子协议
local p_multi_priority = Proto("multi.flags", "MultiProto Flags")

-- 协议字段值解析映射，对于枚举类型非常适合
local falseortrue = {
        [0] = "false",
        [1] = "true"
}

p_multi_priority.fields.priority_value = ProtoField.uint8("multi.flags.priority", "Priority Value", base.DEC)
p_multi_priority.fields.prefix_flag = ProtoField.uint8("multi.flags.prefix", "Prefix Flag", base.DEC, falseortrue)
p_multi_priority.fields.suffix_flag = ProtoField.uint8("multi.flags.suffix", "Suffix Flag", base.DEC, falseortrue)

function p_multi_priority.dissector(buf, pkt, tree)
    -- 协议解析区域，在自身根节点中通过buf的第二个参数指明
    -- 具体此协议，解析一个字节长度
    --- 注意，每次协议解析，buf的偏移都是相对于0开始，buf的第二个参数长度指示选中此节点时，Wireshark高亮的码流区域
    local subtree = tree.add(p_multi_priority, buf(0, 1))
    local u8 = buf(0, 1):uint()

    subtree:add(p_multi_priority.fields.priority_value, buf(0, 1), bit32.band(u8, 0xF))
    subtree:add(p_multi_priority.fields.prefix_flag, buf(0, 1), bit32.rshift(bit32.band(u8, 0x10), 4))
    subtree:add(p_multi_priority.fields.suffix_flag, buf(0, 1), bit32.rshift(bit32.band(u8, 0x20), 5))
end

-- 自身解析层
-- 解析映射将值域为枚举类型的字段，解析为更易懂的文本。对于没有定义映射，将显示为'Unknown (value)'样式
local vs_protos = {
        [2] = "mtp2",
        [3] = "mtp3",
        [4] = "alcap",
        [5] = "h248",
        [6] = "ranap",
        [7] = "rnsap",
        [8] = "nbap"
}

-- 协议字段类型，常用的字段类型为uint*系列，以及stirng、bytes等类型
--- + 第一个参数将用于过滤条件，最好以根协议的缩写命名开头，但具有唯一性即可
--- + 第二个参数为字段描述，在协议解析树中作为节点名字，在协议内要求描述唯一
p_multi.fields.proto = ProtoField.uint8("multi.protocol", "Protocol", base.DEC, vs_protos)
p_multi.fields.dir   = ProtoField.uint8("multi.direction", "Direction", base.DEC, { [1] = "incoming", [0] = "outgoing"})
p_multi.fields.raw   = ProtoField.bytes("multi.text", "Text")


-- 默认待链接解析器，对于不需要深入解析的区域，可以采用码流解析器
local data_dis = Dissector.get("data")

-- 解析器能够识别的链接解析器
local protos = {
        [2] = Dissector.get("mtp2"),
        [3] = Dissector.get("mtp3"),
        [4] = Dissector.get("alcap"),
        [5] = Dissector.get("h248"),
        [6] = Dissector.get("ranap"),
        [7] = Dissector.get("rnsap"),
        [8] = Dissector.get("nbap"),
        [9] = Dissector.get("rrc"),
        -- 注意，解析器的不同获取方式
        [10] = DissectorTable.get("sctp.ppi"):get_dissector(3), -- m3ua
        [11] = DissectorTable.get("ip.proto"):get_dissector(132), -- sctp
}

-- 解析器函数
--- + 第一个参数为报文数据，可以进行切片操作
--- + 第二个参数为wireshar解析行描述对象，可以操作行描述信息
--- + 第三个参数为wireshar当前解析树，可以继续增加新的子树和链接协议器

function p_multi.dissector(buf, pkt, tree)
        -- 修改报文行的协议为已识别的协议
        pkt.cols.protocol:set(p_multi.name)
        
        -- local length = buf:len()

        -- 生成自身解析树
        local subtree = tree:add(p_multi, buf(0, 3))
        local offset = 0

        -- 注： subtree:add_le 可以按照字节序小端来获取报文中的数据
        subtree:add(p_multi.proto, buf(offset, 1))
        offset = offset + 1

        subtree:add(p_multi.dir, buf(offset, 1))
        offset = offset + 1

        -- 调用自身解析树的子协议进行解析
        --- 注意第一个参数需要产生新的tvb
        --- 注意第三个参数为自身子树节点
        p_multi_priority.dissector(buf(offset, 1):tvb(), pkt, subtree)
        offset = offset + 1

        -- 链接其它解析器
        --- 注意树节点参数的不同
        local proto_id  = buf(0,1):uint()
        local dissector = protos[proto_id]

        if dissector ~= nil then
                -- 链接其它可以识别的解析器
                dissector:call(buf(offset):tvb(), pkt, tree)
        elseif proto_id < 2 then
                -- 特殊分支，在自身解析层完成，注意树节点为自身树
                subtree:add(p_multi.raw, buf(offset))
        else
                -- 作为默认链接裸数据解析器
                data_dis:call(buf(offset):tvb(), pkt, tree)
        end

end

local wtap_encap_table = DissectorTable.get("wtap_encap")
local udp_encap_table  = DissectorTable.get("udp.port")

wtap_encap_table:add(wtap.USER15, p_multi)
wtap_encap_table:add(wtap.USER12, p_multi)
udp_encap_table:add(7555, p_multi)
```

# 参考
+ [Dissector Example](https://www.wireshark.org/docs/wsdg_html_chunked/wslua_dissector_example.html)
+ [Proto API](https://www.wireshark.org/docs/wsdg_html_chunked/lua_module_Proto.html)
+ [Tvb API](https://www.wireshark.org/docs/wsdg_html_chunked/lua_module_Tvb.html#lua_class_Tvb)
