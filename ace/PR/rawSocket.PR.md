I have completed the work of RAW Socket by referring `ACE_ICMP_Socket` & `ACE_Sock_Dgram` classes,  these are provided by ACE long time ago.

I have tested the API of RAW Socket on Linux platforms, such as Ubuntu 20.04 and CentOS 7.
On windows platform , there exist some surprise maybe for OS limits, RAW_Socket_Test case can not pass through.

For considering the fact that applications using RAW Socket almost run on Unix* or Linux* platforms,  this failure is trivial!

So I decide to launch the PR :)

---

This PR has been launched some months ago,  but it is based on master branch .
This time I launch a new PR based on a special branch :)