#!/bin/bash

releasever=7-9.2009
os_name=centos$releasever
iso_mount_dir=/mnt/$os_name-iso
image_rootfs_dir=/opt/$os_name-rootfs
image_name=my-$os_name-from-iso:latest
iso_path_name=CentOS-7-x86_64-DVD-2009.iso


# 创建挂载点和目标目录
if [ ! -r $iso_path_name ]; then
  echo "ISO image file: $iso_path_name does not exist, please check!"
  exit
fi

umount $iso_mount_dir
sudo rm -rf $image_rootfs_dir
sudo mkdir $iso_mount_dir $image_rootfs_dir

# 挂载 ISO
sudo mount -o loop $iso_path_name $iso_mount_dir

# 提取必要文件（使用 rpm 安装到目标目录）
echo "[$os_name-local-iso]
name=$os_name-local-iso Repository
baseurl=file://$iso_mount_dir
enabled=1
gpgcheck=0" > /etc/yum.repos.d/$os_name-local-iso.repo
echo "You must keep the yum tool in host machine compatiable for target os"
sudo yum install --releasever=$releasever --disablerepo=\* --enablerepo=$os_name-local-iso --installroot=$image_rootfs_dir centos-release rpm yum-utils 

if [ $? -ne 0 ]; then
  echo "Some error occured when installing the bootstrap components, please check!"
  exit
fi


# 初始化 rpm 数据库
sudo chroot $image_rootfs_dir /bin/bash -c "rpm --initdb"

# 语言设置
sudo chroot $image_rootfs_dir /bin/bash -c "echo LANG=\"en_US.UTF-8\" > /etc/locale.conf"

# 清理部分文件
sudo chroot $image_rootfs_dir /bin/bash -c "localedef --list-archive | grep -v -i "en_US" | xargs localedef --delete-from-archive && mv /usr/lib/locale/locale-archive /usr/lib/locale/locale-archive.tmpl && build-locale-archive"
sudo chroot $image_rootfs_dir /bin/bash -c "yum clean all"

# 打包 rootfs
cd $image_rootfs_dir
yes | rm -f  /tmp/rootfs.tar.gz
sudo tar -cvzf /tmp/rootfs.tar.gz .

# 导入为 Docker 镜像
docker rmi $image_name
cat /tmp/rootfs.tar.gz | docker import - $image_name

# 验证
docker run --rm -it $image_name /bin/bash