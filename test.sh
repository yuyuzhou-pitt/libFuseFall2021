
# ----- SETUP -----

# Exit if any command fails
set -e

# Build program
make
sudo make install

# Create directories for testing
#mkdir bd_test/
#mkdir mp_test/

# Mount directory
#ntapfuse mount bd_test/ mp_test/

# Enter mountpoint directory
#cd mp_test/

# Notify user
echo
echo
echo " --- Setup successful, starting test suite... ---"
echo 
echo
sleep .5

#########################################################################
### SET-UP ##############################################################
#########################################################################

# Create directories for testing
mkdir bd_test/
mkdir mp_test/

# Mount directory
ntapfuse mount bd_test/ mp_test/

#Test make sure bd_test was created
if [ -d bd_test/ ];
then
    echo ""
    echo "bd_test CREATED"
else
    echo "bd_test CREATION FAILURE"
fi

#Test make sure mp_test was created
if [ -d mp_test/ ];
then
    echo "mp_test CREATED"
    echo ""
else
    echo "mp_test CREATION FAILURE"
    echo ""
fi


# Enter mountpoint directory
cd mp_test/

# -------------------- TESTS GO HERE ------------------------

#########################################################################
### TEST 1 - CREATE FILE USING ECHO - NO DATABSE PRESENT ################
#########################################################################

echo "EXECUTING TEST 1!"

echo "1:    Creating numbers file"
echo "1234567812345678123456781234567812345678123456781234567812345678" > numbers

#SLEEP
sleep .5

echo "2:    Checking db file exists"
if [ -a db ];
then
    echo "FILE EXISTS"
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "FILE DOES NOT EXISTS"
fi

echo "DISPLAYING DATABASE:"
echo ""
cat db
echo ""

numbers_size=$(stat --format=%s "numbers")
#echo "DISPLAYING TESTFILE SIZE"
#echo $numbers_size

numbers_user=$(stat -c '%u' "numbers")
#echo $numbers_user

numbers_test_str="${numbers_user} ${numbers_size} 4096"
dbfile="$(cat db)"

expected=$(echo $numbers_test_str)
actual=$(echo $dbfile)

if [[ "$expected" == "$actual" ]] 
then
    echo "TEST 1 SUCCEEDED"
else
    echo "TEST 1 FAILED"
fi

rm db
rm numbers


#########################################################################
### TEST 2 - WRITING APPENDING TO EXISTING FILE #########################
#########################################################################

echo "EXECUTING TEST 2!"

echo "1:    Creating numbers file"
echo "1234567812345678123456781234567812345678123456781234567812345678" > numbers

#SLEEP
sleep .5

echo "2:    Checking db file exists"
if [ -a db ];
then
    echo "FILE EXISTS"
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "FILE DOES NOT EXISTS"
fi

echo "DISPLAYING DATABASE:"
echo ""
cat db
echo ""

numbers_size=$(stat --format=%s "numbers")
#echo "DISPLAYING TESTFILE SIZE"
#echo $numbers_size

numbers_user=$(stat -c '%u' "numbers")
#echo $numbers_user

numbers_test_str="${numbers_user} ${numbers_size} 4096"
dbfile="$(cat db)"

expected=$(echo $numbers_test_str)
actual=$(echo $dbfile)

if [[ "$expected" == "$actual" ]] 
then
    echo "TEST 1 PASSED"
else
    echo "TEST 1 FAILED"
fi
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
