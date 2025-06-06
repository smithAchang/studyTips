Name:           bello
Version:        0.1
Release:        1%{?dist}
Summary:        Hello World example implemented in bash script

License:        GPLv3+
URL:            https://www.example.com/%{name}
Source0:        https://www.example.com/%{name}/releases/%{name}-%{version}.tar.gz

# for rpm --prfix=dir
Prefix: /usr

Requires:       bash

BuildArch:      noarch

%description
The long-tail description for our Hello World Example implemented in
bash script.

%prep
cat > %{name}.sh << EOF
#!/bin/bash
echo "Hello World!"
EOF

# Only script file, no need for building
#%build

%install

mkdir -p %{buildroot}%{_prefix}/bin

install -m 0755 %{name}.sh %{buildroot}%{_prefix}/bin/%{name}.sh

%files
%{_prefix}/bin/%{name}.sh

%post
#!/bin/sh
echo "Post step after installed"
#env
echo "enviroment 'RPM_INSTALL_PREFIX' var value:$RPM_INSTALL_PREFIX"
cat $RPM_INSTALL_PREFIX/bin/%{name}.sh

%changelog
* Tue May 31 2025 chang.yunlei <changyunlei@fedoraproject.org> - 0.1-1
- First bash hello package