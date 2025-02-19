# Display statistics about the number of loose objects and their disk usage in a human-readable format
git count-objects -vH 

# Expire all reflog entries immediately (removes references to old commits)
git reflog expire --expire=now --all

# Repack the repository, removing duplicate objects and packing loose objects into a single pack file
git repack -ad

# Remove unreachable objects that are no longer referenced by any branch or tag
git prune

# Display updated statistics about the number of loose objects and their disk usage after cleanup
git count-objects -vH
