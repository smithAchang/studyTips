
# 场景
用惯了`vi`命令的`[num]dd`命令来删除一行或多行，但是，在代码编辑IDE工具`Source Insight3`中，竟然在`Edit`菜单里面，找不到删除代码行的操作。

一般编辑工具这种操作菜单，都会集中在`Edit`菜单中，一直错以为`si3`不提供这样的操作。

这两天遇到一个情况，要反复删除某些行，以用来测试修改后的结果，那么，通过移动光标选中某行加`delete`按键，或一直操作`Backspace`退格的方法，就远远不方便了。

# 突破

突破来自痛苦后的淡定思考，为什么不搜索下`si3`现在支持多少快捷命令呢？

打开`si3-->Options-->Key Assignments`菜单项，在打开的窗口中搜索`delete`关键词，发现候选项中存在`Edit: delete line`的命令，但是，未提供快捷键盘命令。

设置了`Edit: delete line`快捷按键`Ctrl + d`后，就可以丝滑地删除一行了。

虽然，没有删除多行的操作，但，已经比以前方便多了！

>另外，打开`si3-->Options-->Menu Assignments`菜单项，可以将删除行的命令集成在语义相近的拷贝、粘贴、复制的菜单项附近，也就可以通过菜单进行操作了。

#  其它
通过此方法搜索其它关键词，或可以发现其它有用的操作，就可以将`si3`定制的更好 :)
