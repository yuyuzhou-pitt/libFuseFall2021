
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
    #echo ""
    #echo "bd_test CREATED"
    :
else
    echo "bd_test CREATION FAILURE"
fi

#Test make sure mp_test was created
if [ -d mp_test/ ];
then
    #echo "mp_test CREATED"
    #echo ""
    :
else
    echo "mp_test CREATION FAILURE"
fi


# Enter mountpoint directory
cd mp_test/

# -------------------- TESTS GO HERE ------------------------

#########################################################################
### TEST 1 - CREATE FILE USING ECHO - NO DATABSE PRESENT ################
#########################################################################

echo ""
echo "########################################################################
### TEST 1 - CREATE FILE USING ECHO - NO LOG PRESENT ###################
########################################################################"

#echo "1:    Creating numbers file"
echo "1234567812345678123456781234567812345678123456781234567812345678" > numbers

#SLEEP
sleep .5

echo "2:    Checking db file exists"
if [ -a db ];
then
    #echo "FILE EXISTS"
    :
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "LOG FILE DOES NOT EXISTS"
    echo ""
fi

#echo "DISPLAYING LOG FILE:"
#echo ""
#cat log
#echo ""

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
echo ""


#########################################################################
### TEST 2 - APPENDING TO EXISTING FILE #################################
#########################################################################

echo ""
echo "########################################################################
### TEST 2 - APPENDING TO EXISTING FILE ################################
########################################################################"


#################################################### CREATE FILE ######################################################
#echo "1:    Creating numbers file"
echo "1234567812345678123456781234567812345678123456781234567812345678" > numbers

#SLEEP
sleep .5

echo "2:    Checking db file exists"
if [ -a db ];
then
    #echo "FILE EXISTS"
    :
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "LOG FILE DOES NOT EXISTS"
    echo ""
fi

#echo "DISPLAYING LOG FILE:"
#echo ""
#cat log
#echo ""

numbers_size=$(stat --format=%s "numbers")
#echo "DISPLAYING TESTFILE SIZE"
#echo $numbers_size

numbers_user=$(stat -c '%u' "numbers")
#echo $numbers_user

numbers_test_str="${numbers_user} ${numbers_size} 4096"
logfile="$(cat log)"

expected=$(echo $numbers_test_str)
actual=$(echo $logfile)

if [[ "$expected" == "$actual" ]] 
then
    :
else
    echo "FIRST FILE WRITE FAILED"
fi

#################################################### APPEND TO FILE ######################################################

echo "0987654321" >> numbers

numbers_size=$(stat --format=%s "numbers")
#echo "DISPLAYING TESTFILE SIZE"
#echo $numbers_size

numbers_user=$(stat -c '%u' "numbers")
#echo $numbers_user

numbers_test_str="${numbers_user} ${numbers_size} 4096"
logfile="$(cat log)"
#cat log

expected=$(echo $numbers_test_str)
actual=$(echo $logfile)

if [[ "$expected" == "$actual" ]] 
then
    echo "TEST 2 SUCCEEDED"
    :
else
    echo "TEST 2 FAILED"
fi

rm numbers
rm log

echo ""

#########################################################################
### TEST 3 - CREATING TWO DIFFERENT FILES WITH SAME USER ################
#########################################################################

echo "########################################################################
### TEST 3 - CREATING TWO DIFFERENT FILES WITH SAME USER ###############
########################################################################"


#################################################### CREATE 1ST FILE ######################################################
#echo "1:    Creating numbers file"
echo "1234567812345678123456781234567812345678123456781234567812345678" > numbers

#SLEEP
sleep .5

#echo "2:    Checking db file exists"
if [ -a log ];
then
    #echo "FILE EXISTS"
    :
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "LOG FILE DOES NOT EXISTS"
    echo ""
fi

#echo "DISPLAYING LOG FILE:"
#echo ""
#cat log
#echo ""

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
    :
else
    echo "FIRST FILE WRITE FAILED"
fi

#################################################### CREATE 2ND FILE ######################################################

#echo "1:    Creating numbers file"
echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ" > letters

#SLEEP
sleep .5

#echo "2:    Checking db file exists"
if [ -a log ];
then
    #echo "FILE EXISTS"
    :
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "LOG FILE DOES NOT EXISTS"
    echo ""
fi

#echo "DISPLAYING LOG FILE:"
#echo ""
#cat log
#echo ""

letters_size=$(stat --format=%s "letters")
#echo "DISPLAYING TESTFILE SIZE"
#echo $letters_size

letters_user=$(stat -c '%u' "letters")
#echo $numbers_user

if [[ "$letters_user" == "$numbers_user" ]] 
then
    #echo "TEST 1 SUCCEEDED"
    :
else
    echo "LETTERS AND NUMBERS USERS ARE NOT EQUAL"
fi

updated_size=$(($letters_size + $numbers_size))

test_str="${letters_user} ${updated_size} 4096"
logfile="$(cat log)"

#echo "$test_str"
#echo "$logfile"

expected=$(echo $test_str)
actual=$(echo $logfile)

if [[ "$expected" == "$actual" ]] 
then
    echo "TEST 3 SUCCEEDED"
    :
else
    echo "TEST 3 FAILED"
fi

rm numbers
rm letters 
rm log

echo ""

#########################################################################
### TEST 4 - REMOVING THE ONLY FILE  ####################################
#########################################################################

echo ""
echo "########################################################################
### TEST 4 - REMOVING THE ONLY FILE ####################################
########################################################################"


#################################################### CREATE FILE ######################################################
#echo "1:    Creating numbers file"
echo "1234567812345678123456781234567812345678123456781234567812345678" > numbers

#SLEEP
sleep .5

#echo "2:    Checking db file exists"
if [ -a log ];
then
    #echo "FILE EXISTS"
    :
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "LOG FILE DOES NOT EXISTS"
    echo ""
fi

#echo "DISPLAYING LOG FILE:"
#echo ""
#cat log
#echo ""

numbers_size=$(stat --format=%s "numbers")
#echo "DISPLAYING TESTFILE SIZE"
#echo $numbers_size

numbers_user=$(stat -c '%u' "numbers")
#echo $numbers_user

numbers_test_str="${numbers_user} ${numbers_size} 4096"
logfile="$(cat log)"

expected=$(echo $numbers_test_str)
actual=$(echo $logfile)

if [[ "$expected" == "$actual" ]] 
then
    :
else
    echo "FIRST FILE WRITE FAILED"
fi

#################################################### REMOVE FILE ######################################################

rm numbers

numbers_test_str="${numbers_user} 0 4096"
logfile="$(cat log)"

#echo "$numbers_test_str"
#echo "$logfile"

expected=$(echo $numbers_test_str)
actual=$(echo $logfile)

if [[ "$expected" == "$actual" ]] 
then
    echo "TEST 4 SUCCEEDED"
    :
else
    echo "TEST 4 FAILED"
fi

rm log

echo ""


#########################################################################
### TEST 5 - CREATING MULTIPLE FILES AND REMOVING ONE ###################
#########################################################################

echo ""
echo "########################################################################
### TEST 5 - CREATING MULTIPLE FILES AND REMOVING ONE ##################
########################################################################"


#################################################### CREATE 1ST FILE ######################################################
#echo "1:    Creating numbers file"
echo "1234567812345678123456781234567812345678123456781234567812345678" > numbers

#SLEEP
sleep .5

#echo "2:    Checking db file exists"
if [ -a log ];
then
    #echo "FILE EXISTS"
    :
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "LOG FILE DOES NOT EXISTS"
    echo ""
fi

#echo "DISPLAYING LOG FILE:"
#echo ""
#cat log
#echo ""

numbers_size=$(stat --format=%s "numbers")
#echo "DISPLAYING TESTFILE SIZE"
#echo $numbers_size

numbers_user=$(stat -c '%u' "numbers")
#echo $numbers_user

numbers_test_str="${numbers_user} ${numbers_size} 4096"
logfile="$(cat log)"

#echo "$numbers_test_str"
#echo "$logfile"

expected=$(echo $numbers_test_str)
actual=$(echo $logfile)

if [[ "$expected" == "$actual" ]] 
then
    :
else
    echo "FIRST FILE WRITE FAILED"
fi

#################################################### CREATE 2ND FILE ######################################################

#echo "1:    Creating numbers file"
echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ" > letters

#SLEEP
sleep .5

#echo "2:    Checking db file exists"
if [ -a log ];
then
    #echo "FILE EXISTS"
    :
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "LOG FILE DOES NOT EXISTS"
    echo ""
fi

#echo "DISPLAYING LOG FILE:"
#echo ""
#cat log
#echo ""

letters_size=$(stat --format=%s "letters")
#echo "DISPLAYING TESTFILE SIZE"
#echo $letters_size

letters_user=$(stat -c '%u' "letters")
#echo $numbers_user

if [[ "$letters_user" == "$numbers_user" ]] 
then
    #echo "TEST 1 SUCCEEDED"
    :
else
    echo "LETTERS AND NUMBERS USERS ARE NOT EQUAL"
fi

updated_size=$(($letters_size + $numbers_size))

test_str="${letters_user} ${updated_size} 4096"
logfile="$(cat log)"

#echo "$test_str"
#echo "$logfile"

expected=$(echo $test_str)
actual=$(echo $logfile)

if [[ "$expected" == "$actual" ]] 
then
    :
else
    echo "LOG FILE NOT UPDATED FOR SECOND FILE CREATION"
fi

#################################################### REMOVE 1ST FILE ######################################################

rm numbers

updated_size=$(($updated_size - $numbers_size))

test_str="${letters_user} ${updated_size} 4096"
logfile="$(cat log)"

#echo "$test_str"
#echo "$logfile"

expected=$(echo $test_str)
actual=$(echo $logfile)

if [[ "$expected" == "$actual" ]] 
then
    echo "TEST 5 SUCCEEDED"
    :
else
    echo "TEST 5 FAILED"
fi

rm letters 
rm log

echo ""

#########################################################################
### TEST 6 - CREATING MULTIPLE FILES WITH DIFFERENT USERS ###############
#########################################################################

echo ""
echo "########################################################################
### TEST 6 - CREATING MULTIPLE FILES WITH DIFFERENT USERS ##############
########################################################################"

#################################################### CREATE FIRST FILE WITH ROOT ######################################################

#echo "1:    Creating numbers file"
echo "1234567812345678123456781234567812345678123456781234567812345678" > numbers

#SLEEP
sleep .5

#echo "2:    Checking db file exists"
if [ -a log ];
then
    #echo "FILE EXISTS"
    :
else
    echo "!!!!!!!!!!!!!!!!!!!!"
    echo "LOG FILE DOES NOT EXISTS"
    echo ""
fi

#echo "DISPLAYING LOG FILE:"
#echo ""
#cat log
#echo ""

numbers_size=$(stat --format=%s "numbers")
#echo "DISPLAYING TESTFILE SIZE"
#echo $numbers_size

numbers_user=$(stat -c '%u' "numbers")
#echo $numbers_user

numbers_test_str="${numbers_user} ${numbers_size} 4096"
logfile="$(cat log)"

expected=$(echo $numbers_test_str)
actual=$(echo $logfile)

if [[ "$expected" == "$actual" ]] 
then
    echo "TEST 6 SUCCEEDED"
else
    #echo "TEST 6 FAILED"
    :
fi

#################################################### CREATE ANOTHER USER ######################################################

sudo useradd testuser -G sudo

cd ..

expect testuser.exp

su testuser

echo "TEST USER RUNNING"

cd mp_test

#################################################### CREATE SECOND FILE ######################################################


rm log
rm numbers
echo ""

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
