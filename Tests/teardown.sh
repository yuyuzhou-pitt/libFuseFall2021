# Notify user
echo
echo
echo " --- Cleaning up... ---"
echo 
echo
sleep .5

cd ..

# Unmount directory
sudo umount mp_test/

sudo pkill -KILL -u testuser
sudo userdel -r testuser

# Remove test directories
rm -rf bd_test/
rm -rf mp_test/