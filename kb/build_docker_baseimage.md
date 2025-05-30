# 引言
这段最值得总结的经验知识,就是如何在ISO镜像的基础上，直接做出`docker base image`，无需联网!

特别地，对于一些老旧`OS`系统，都能依此做出`docker base image`！

例如,某些老旧系统，`CentOS 6`，有些时候需要旧的编译环境和运行环境。

但，对于`CentOS 6`，*docker hub*官网上没有提供docker基础镜像，那么怎么办呢？
# 制作容器镜像文件系统环境准备
+ 从ISO镜像安装一台物理机或虚机
+ 将ISO镜像文件拷贝进入主机硬盘
## 制作容器镜像准备
+ 在支持`docker`的`OS`主机上安装`docker`环境

> 注意，制作`docker`镜像的环境，可以制作`docker base image`文件系统的`OS`不同，即与ISO镜像中的`OS`，可以存在不同
# 制作要点

+ 制作`docker`基础镜像的文件系统，类似于编译类型中的`本地编译`，强烈不建议用~~交叉编译~~

+ 利用`rpm -qf /path/to/component_file`，可以反向查阅**安装文件**来自于哪个组件；这样可以确定`docker`基础镜像那些是需要的必要组件

+ 必须安装**rootfiles**组件，以利于打开从镜像创建容器的`shell`，可以自动加载`colorls.sh`等易用性脚本

+ 安装**centos-release**组件，以利于形成正确的`/etc/os-release`等文件
> 注意，`/etc/*-release文件`， *OS*不同，文件名字亦不同，但均可进行反向查阅，进行确定来源的安装组件

# 执行脚本

+ *希望代码如文字介绍，不赘述*


```bash
#!/bin/bash

releasever=7-9.2009
os_name=centos$releasever
iso_mount_dir=/mnt/$os_name-iso
image_rootfs_dir=/opt/$os_name-rootfs
image_name=my-$os_name-from-iso:latest
iso_path_name=/path/to/CentOS-7-x86_64-DVD-2009.iso


# 创建挂载点和目标目录
if [ ! -r $iso_path_name ]; then
  echo "ISO image file: $iso_path_name does not exist, please check!"
  exit
fi

# 制作前清理和建立目录
echo "Clean before making the 'docker' base image file system"
sudo umount $iso_mount_dir
sudo rm -rf $image_rootfs_dir
sudo mkdir $iso_mount_dir $image_rootfs_dir

## 挂载 ISO
sudo mount -o loop $iso_path_name $iso_mount_dir

# 提取必要组件，特别是'rpm、yum、bash、glibc'
sudo echo "[$os_name-local-iso]
name=$os_name-local-iso Repository
baseurl=file://$iso_mount_dir
enabled=1
gpgcheck=0" > /etc/yum.repos.d/$os_name-local-iso.repo

# 需要保持制作'docker'基础镜像文件系统的'OS'与目标一致，以利于形成正确的'yum'环境信息，有利于后期在此基础上继续安装其它新组件
echo -e "You must keep the yum tool in the host machine compatible for the target os.\nYou can enter 'N' to exit the process"
sudo yum install --releasever=7 --disablerepo=\* --enablerepo=$os_name-local-iso --installroot=$image_rootfs_dir rootfiles centos-release rpm yum-utils vim-minimal

if [ $? -ne 0 ]; then
  echo "Some error occured when installing the bootstrap components, please check!"
  exit
fi

# 初始化 rpm 数据库
sudo chroot $image_rootfs_dir /bin/bash -c "rpm --initdb"

# 语言设置
sudo chroot $image_rootfs_dir /bin/bash -c "echo LANG=\"en_US.UTF-8\" > /etc/locale.conf"

# 清理部分文件精简基础镜像大小
echo "Clean some unused files to reduce the 'docker' base image size"
sudo chroot $image_rootfs_dir /bin/bash -c "localedef --list-archive | grep -v -i "en_US" | xargs localedef --delete-from-archive && mv /usr/lib/locale/locale-archive /usr/lib/locale/locale-archive.tmpl && build-locale-archive"
sudo chroot $image_rootfs_dir /bin/bash -c "yum clean all; rm -rf /var/cache/yum /etc/yum.repos.d/$os_name-local-iso.repo"

# 打包 rootfs
cd $image_rootfs_dir
yes | rm -f  /tmp/rootfs.tar.gz
tar -cvzf /tmp/rootfs.tar.gz .

# 在支持'docker'的环境中导入为'docker'镜像
echo "You may import the tar file at another machine if the environment making the file system can't run 'docker tool chains' well"
docker rmi $image_name
cat /tmp/rootfs.tar.gz | docker import - $image_name

# 启动镜像容器验证是否可用
docker run --rm -h isItOK -it $image_name /bin/bash
```
# 参考
+ DeepSeek等辅助工具
+ [how build a docker base image from a ISO image](123)