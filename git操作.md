推送本地分支local_branch到远程分支 remote_branch并建立关联关系

      a.远程已有remote_branch分支并且已经关联本地分支local_branch且本地已经切换到local_branch

          git push

     b.远程已有remote_branch分支但未关联本地分支local_branch且本地已经切换到local_branch

         git push -u origin/remote_branch

     c.远程没有remote_branch分支并，本地已经切换到local_branch

        git push origin local_branch:remote_branch

---
# 在本地创建分支dev并切换到该分支
git fetch origin ringQueue


git checkout -b dev(本地分支名称) origin/dev(远程分支名称)
git checkout -b branchname origin/branchname

-----

git clone -b v3.10 git@github.com:torvalds/linux.git

-----
git stash pop
git stash list

----切换到主分支
git checkout master
git  merge dev

--推向远端
git push origin master

更新远程分支列表
git remote update origin --prune

查看所有分支
git branch -a

删除远程分支Chapater6
git push origin --delete Chapater6

删除本地分支 Chapater6
git branch -d  Chapater6

--在分支上建标签，预先切换到某一分支
git tag -a v1.4 -m "my version 1.4"

--- 根据历史打标签
git log --pretty=oneline
git tag -a v1.2 9fceb02
git checkout -b branch_name tagname


--推某一个标签
git push origin v1.5
--推所有标签

git push origin --tags
