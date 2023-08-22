
sudo docker run --rm  -it --name aligncgsl5 -v ~/workspace/:/workspace -h aligncgsl5 -w /workspace centos:centos7.6.1810 /bin/bash


sudo docker run -it --name aligncgsl5 -v ~/workspace/:/workspace -h aligncgsl5 -w /workspace centos:centos7.6.1810 /bin/bash
sudo docker create -it --name aligncgsl5 -v ~/workspace/:/workspace -h aligncgsl5 -w /workspace centos:centos7.6.1810 /bin/bash

sudo docker start -i aligncgsl5

在docker中安装vpp还要辅助安装git \make\sudo

http://github.com/01org/intel-ipsec-mb

./extras/vagrant/build.sh

make install-deps
make install-ext-deps


sudo docker run --rm  -it --name gcc11.2.0 -v ~/workspace/:/workspace -h gcc11.2.0 -w /workspace gcc:11.2.0 /bin/bash


导出镜像
如果要存出镜像到本地文件，可以使用docker save命令。例如，存出本地的ubuntu:14.04镜像为文件ubuntu_14.04.tar：
$ sudo docker save -o /home/user/images/ubuntu_14.04.tar ubuntu:14.04

导入镜像
可以使用docker load从存出的本地文件中再导入到本地镜像库，例如从文件ubuntu_14.04.tar导入镜像到本地镜像列表，如下所示：
$ sudo docker load --input ubuntu_14.04.tar



