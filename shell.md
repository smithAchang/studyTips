
# replace the Makefile
find ~/gitrepo/studyTips -name Makefile | xargs -I {} sed -i 's/-Wall/-Wall -std=gnu11/' {}
