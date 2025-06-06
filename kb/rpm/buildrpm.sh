#!/bin/bash
#

# yum install rpmdevtools
rpmdev-setuptree

cp -f bello.spec ~/rpmbuild/SPECS

# build binary RPM
rpmbuild -bb ~/rpmbuild/SPECS/bello.spec

# 
rpm -qp --scripts ~/rpmbuild/RPMS/noarch/bello-0.1-1.*.rpm