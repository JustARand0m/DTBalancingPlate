#!/usr/bin/env bash

lineByLine() {
  filename="$1"
  while read line; do
  # reading each line
  echo "$line"
  sleep $2
  done < $filename
}

dotWait() {
  for i in {1..4}
  do 
    echo -e "."
    sleep 0.5
  done
}

splashScreen() {
  clear
  lineByLine "logo1.txt" 0.03
  #echo -e "\n\t\t\t\t    PRESENTS"
  dotWait
  clear
  lineByLine "logo2.txt" 0.04
  dotWait
  clear
  lineByLine "logo3.txt" 0.02
  dotWait
  clear
}


#####################################
echo -e "\n > go.sh started \n"
sleep 1

if [ "$1" == "-s" ]; then
  splashScreen
fi

# Start Balancing
echo -e " > Starting Plate - Korbinian Stuff todo\n"
sleep 0.5


# Print QR and IP
echo -e " > Printing QR Code and IP\n"
sleep 0.3

# macOS
ipconfig getifaddr en0 | awk '{print $1}' | qr
ipconfig getifaddr en0 | awk '{print "BalancingPlate IP: "$1"\n"}'

# raspi / linux
#hostname -I | awk '{print $1}' | qr
#hostname -I | awk '{print $1}' | xargs echo "BalancingPlate IP: $1"


# Start ReceiverScript listening on Port 1337
echo -e " > Starting ReceiverScript\n"
sleep 0.3
python receive.py

echo -e ""



