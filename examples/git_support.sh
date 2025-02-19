git count-objects -vH 
git reflog expire --expire=now --all
git repack -ad
git prune
git count-objects -vH 
