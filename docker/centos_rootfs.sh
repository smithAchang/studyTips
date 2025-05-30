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