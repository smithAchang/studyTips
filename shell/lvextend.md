
# 过去式
以前磁盘管理基于~~磁盘、分区、挂载点~~的管理方式，相对比较老旧...

首先，用`fdisk <disk>`命令进行创建分区，然后用`mkfs.xfs <devicename> `建立`xfs`文件系统，最后再用`mount devicename mountpoint_directory`命令，将分区挂载到文件系统中的某个挂载点，就可以使用了。

同时，为了方便在启动时自动完成加载，还需要深入编辑`/etc/fstab`进行配置。

不过，此种磁盘扩容方式已经是旧时代的操作方式，也很难对于某个分区进行扩容，特别是某个分区划分比较小的时间，就更痛苦了！

建议使用`LVM`以卷为操作对象的现代磁盘分区管理技术，实现方便的**在线 online**磁盘扩容！

---
# 基于LVM逻辑盘卷管理的体系
## VG
+ PV*
+ ...
> disk device* -> PV*

## VG                                   
+ LV1
+ + PV*
+ + ...
+ LV2
 + + PV*
 + + ...
 + LV*
 + + ...
 
 ##  概念解释
> 概念上分为设备、物理卷、卷组、逻辑卷；最终呈现给用户使用的为逻辑卷**LV**。
>  当LV对应的磁盘容量比较小时，可以将新磁盘作为新的物理卷加入此**LV**就可以实现扩容！

---                    

# 观察磁盘分区
```bash
fdisk -l
```

> 注意分两次观察，可以观察出来是否磁盘增加

# 扩容
## 对新增硬盘创建初始化物理卷
```bash
# usage:   pvcreate PV
# example
pvcreate /dev/sdb
```

### 异常补充
`如果遭遇错误提示`
> Device /dev/sdb excluded by a filter

一般需要重建磁盘的分区，输入命令：
```bash
parted /dev/sdb
输入：mklabel msdos
输入： Yes
输入：quit
```
重建就完成了！

## 查看已有卷组名
```bash
# example
vgs 
```

## 扩展物理卷到卷组
```bash
# usage: vgextend VG PV
# example
vgextend ncl /dev/sdb
```

## 查看已有逻辑卷
```bash
lvs
```

## 扩宽某一逻辑卷大小
```bash
# usage: lvextend  [/dev/mapper| /dev/centos]/VG-LV PV
# example
lvextend /dev/mapper/ncl-root  /dev/sdb
```

## 按照文件系统类型增长旧逻辑卷
```bash
# usage:   xfs_growfs mountpoint
# example
xfs_growfs /dev/mapper/ncl-root
```
# 查看文件系统是否扩容成功
```bash
df -h
```






