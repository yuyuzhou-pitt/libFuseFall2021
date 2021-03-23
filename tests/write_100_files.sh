#!/bin/bash

# setup
rm ../log.txt

# test
for n in {1..100}
do
    echo "this is file #$n" > "file_$n.txt"
    cat "file_$n.txt"
done

expected="write: 
	PATH: /file_1.txt
	SIZE: 16
	OFFS: 0
write: 
	PATH: /file_2.txt
	SIZE: 16
	OFFS: 0
write: 
	PATH: /file_3.txt
	SIZE: 16
	OFFS: 0
write: 
	PATH: /file_4.txt
	SIZE: 16
	OFFS: 0
write: 
	PATH: /file_5.txt
	SIZE: 16
	OFFS: 0
write: 
	PATH: /file_6.txt
	SIZE: 16
	OFFS: 0
write: 
	PATH: /file_7.txt
	SIZE: 16
	OFFS: 0
write: 
	PATH: /file_8.txt
	SIZE: 16
	OFFS: 0
write: 
	PATH: /file_9.txt
	SIZE: 16
	OFFS: 0
write: 
	PATH: /file_10.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_11.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_12.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_13.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_14.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_15.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_16.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_17.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_18.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_19.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_20.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_21.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_22.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_23.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_24.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_25.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_26.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_27.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_28.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_29.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_30.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_31.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_32.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_33.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_34.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_35.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_36.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_37.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_38.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_39.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_40.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_41.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_42.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_43.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_44.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_45.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_46.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_47.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_48.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_49.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_50.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_51.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_52.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_53.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_54.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_55.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_56.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_57.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_58.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_59.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_60.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_61.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_62.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_63.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_64.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_65.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_66.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_67.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_68.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_69.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_70.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_71.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_72.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_73.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_74.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_75.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_76.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_77.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_78.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_79.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_80.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_81.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_82.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_83.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_84.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_85.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_86.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_87.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_88.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_89.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_90.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_91.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_92.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_93.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_94.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_95.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_96.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_97.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_98.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_99.txt
	SIZE: 17
	OFFS: 0
write: 
	PATH: /file_100.txt
	SIZE: 18
	OFFS: 0"
actual=$(cat ../log.txt)

if [[ $actual == $expected ]]; then
    echo "0"
else
    echo "1"
fi

# takedown
for n in {1..100}
do
    rm "file_$n.txt"
done