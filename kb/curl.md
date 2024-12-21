# curl断点续传
访问外网资源，特别是Github上比较大的资源，例如，笔者遇到的`calico`发布包，经常会遇到在浏览器上下载半途中断。

那么支持断点续传的下载工具，就是应对这种情况的好帮手！

简单的断点续传工具，就是`shell`工具箱中的`curl`！

它既支持断点续传，还支持尝试次数的指定，那么尝试次数就用喜欢的`65535`，让它昏天地暗吧！

## 脚本实例
```bash
 curl -C - -LO --retry 65535 --retry-delay 60  [--retry-all-errors  --retry-connrefused] https://github.com/projectcalico/calico/releases/download/v3.27.5/release-v3.27.5.tgz
 # 记得下载校验和文件进行比对
 curl -C - -LO --retry 65535 --retry-delay 60  [--retry-all-errors  --retry-connrefused] https://github.com/projectcalico/calico/releases/download/v3.27.5/SHA256SUMS
 cat SHA256SUMS
 sha256sum release-v3.27.5.tgz
```
## 选项说明


|选项|说明|
|--|--|
|-C -| 支持断点续传的选项|
|--retry*| 支持尝试次数、以及在什么情况下尝试的选项|

+ 某些低版本`curl`不支持`[--retry-all-errors  --retry-connrefused]`，可以用`curl --help all`进行查看是否支持选项

# 推荐工具
+ [非常好用的脚本在线解释器](https://www.explainshell.com/)
+ [Git Bash SHell Using in Windows](https://gitforwindows.org/)