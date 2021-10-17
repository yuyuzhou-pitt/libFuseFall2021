
# ----- SETUP -----

# Exit if any command fails
set -e

# Build program
make
sudo make install

# Create directories for testing
mkdir bd_test/
mkdir mp_test/

# Mount directory
ntapfuse mount bd_test/ mp_test/

# Enter mountpoint directory
cd mp_test/

# Notify user
echo
echo
echo " --- Setup successful, starting test suite... ---"
echo 
echo
sleep .5


# -------------------- TESTS GO HERE ------------------------

#########################################################################
### TEST 1 - CREATE FILE ################################################
#########################################################################

echo "1234567812345678123456781234567812345678123456781234567812345678" > numbers
# TODO: Add sql database access to check this


#########################################################################
### TEST 2 - DELETE FILE ################################################
#########################################################################

rm numbers
# TODO: Add sql database access to check this


#########################################################################
### TEST 3 - MULTIPLE USERS WRITING FILES (NON CONCURRENT) ##############
#########################################################################








# ------------------- TESTS END HERE ------------------------


# ----- CLEANUP -----

# Notify user
echo
echo
echo " --- Cleaning up... ---"
echo 
echo
sleep .5

# Exit mountpoint directory
cd ..

# Unmount directory
sudo umount mp_test/

# Remove test directories
rm -rf bd_test/
rm -rf mp_test/
