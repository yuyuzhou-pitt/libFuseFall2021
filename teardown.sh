# Notify user
echo
echo
echo " --- Cleaning up... ---"
echo 
echo
sleep .5

# Unmount directory
sudo umount mp_test/

# Remove test directories
rm -rf bd_test/
rm -rf mp_test/