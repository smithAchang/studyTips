Name:       higher-gcc
Version:    10.5.0
Release:    1%{?dist}
Summary:    You can use higher gcc version in the same host
License:    Public
Prefix: /opt/gcc/10.5.0

Source0: %{name}-%{version}.tar.gz

BuildRequires: make gcc
BuildArch: x86_64

# Dependency: 'as' program
Requires: binutils

%description
You can use higher gcc version in the same host.

%global objdir %{_builddir}/build
%global srcdir %{_builddir}/gcc-%{version}
%global debug_package %{nil}
%global __debug_install_post %{nil}
#%global __arch_install_post %{nil}
#%global __os_install_post %{nil}

# Reset the global var 
%global _prefix %{prefix}

# For Building env
%prep
%setup -q -n gcc-%{version}

%build
rm -rf %{objdir}
mkdir %{objdir}
cd %{objdir}
# Avoid the default dir '/usr/local'
%{srcdir}/configure --host=x86_64-pc-linux-gnu --prefix=/usr --disable-multilib --enable-languages=c,c++
make -j8

#without var substitute
cat > enable << 'EOF'
#!/bin/sh
GCC_ROOT=/opt/gcc/10.5.0/root
export PATH=$GCC_ROOT/usr/bin${PATH:+:${PATH}}
export MANPATH=$GCC_ROOT/usr/share/man:${MANPATH}
export INFOPATH=$GCC_ROOT/usr/share/info${INFOPATH:+:${INFOPATH}}
export PCP_DIR=$GCC_ROOT
# bz847911 workaround:
# we need to evaluate rpm's installed run-time % { _libdir }, not rpmbuild time
# or else /etc/ld.so.conf.d files?
rpmlibdir=$(rpm --eval "%{_libdir}")
# bz1017604: On 64-bit hosts, we should include also the 32-bit library path.
if [ "$rpmlibdir" != "${rpmlibdir/lib64/}" ]; then
  rpmlibdir32=":$GCC_ROOT${rpmlibdir/lib64/lib}"
fi
export LD_LIBRARY_PATH=$GCC_ROOT$rpmlibdir$rpmlibdir32${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
export LD_LIBRARY_PATH=$GCC_ROOT$rpmlibdir$rpmlibdir32:$GCC_ROOT$rpmlibdir/dyninst$rpmlibdir32/dyninst${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
export PKG_CONFIG_PATH=$GCC_ROOT/usr/lib64/pkgconfig${PKG_CONFIG_PATH:+:${PKG_CONFIG_PATH}}

EOF


%install
mkdir -p %{buildroot}%{_prefix}

cd %{objdir}
make install DESTDIR=%{buildroot}%{_prefix}/root
install -m 755 enable %{buildroot}%{_prefix}/enable
ln -sf gcc %{buildroot}%{_prefix}/root/usr/bin/cc
find %{buildroot}%{_prefix}/root -size +200M | xargs strip

%files
%{_prefix}/enable
%{_prefix}/root

%clean
echo "Clean step"

%post
sed -i "s|^GCC_ROOT=.*|GCC_ROOT=$RPM_INSTALL_PREFIX/root|" $RPM_INSTALL_PREFIX/enable

%changelog
* Wed Mar 5 2025 changyunlei <chang.yunlei1@caltta.com> - 1.0
- create
