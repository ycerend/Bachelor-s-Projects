#!\bin\bash
pkill -f "nc -l"
echo Enter a nickname
read line
my_ip=$(ifconfig en0 | grep "inet " | cut -d " " -f2)
discoverPeople(){
    
    
    other_ips=$(ifconfig en0 | grep "inet " | cut -d " " -f2 | cut -d "." -f1-3)
    for i in {2..254}; do
    
    echo "0;$my_ip;$line;0;0" | nc -G 1 "$other_ips.$i" 5000 &
    done

}
peopledDiscoversYou(){
    
    nc -lk 5000 | while IFS=, read -a p
        do 
        message=${p[0]}
        local type=$(echo $message | cut -d ";" -f1)
        local source_ip=$(echo $message | cut -d ";" -f2)
        local source_name=$(echo $message | cut -d ";" -f3)
        if [ "$type" == "0" ]; then
            echo "1;$my_ip;$line;$source_ip;$source_name" | nc -G 1 "$source_ip" 5000
            echo "$source_ip $source_name" >> people.txt
            sed '/^\s*$/d' people.txt > filetemp2.txt
            rm people.txt
            touch people.txt
            cat filetemp2.txt >> people.txt
            rm filetemp2.txt
        elif [ "$type" == "1" ]; then
            echo "$source_ip $source_name" >> people.txt
            sed '/^\s*$/d' people.txt > filetemp2.txt
            rm people.txt
            touch people.txt
            cat filetemp2.txt >> people.txt
            rm filetemp2.txt


            
        fi
    done
    
}
firstMessage(){
    echo "first message [1] or reply message [2]"
    read choice
    if [ "$choice" == "1" ] ; then
    cat people.txt
    echo " "
    echo "Select people: " 
    read ip_line
    local txt_line=$(sed -n ${ip_line}p people.txt)
    local receiver_ip=$(echo $txt_line | cut -d " " -f1)
    echo "Type Message: " 
    read messageToSend
    local cypher=$(md5 -s $messageToSend)
    local real_cypher=$(echo $cypher | cut -d " " -f4)
    echo "$my_ip;$real_cypher;$messageToSend" | nc $receiver_ip 5001 &
    echo "$receiver_ip $real_cypher $messageToSend" >> cypher_ip.txt

    elif [ "$choice" == "2" ] ; then
    cat sender_cypher_ip.txt
    echo " "
    echo "Select people: " 
    read choice_line
    local txt_line2=$(sed -n ${choice_line}p sender_cypher_ip.txt)
    local receiver_ip2=$(echo $txt_line2 | cut -d " " -f1)
    local receiver_cypher2=$(echo $txt_line2 | cut -d " " -f2)
    local message2=$(echo $txt_line2 | cut -d " " -f3)

    local history_ip=$(grep -n "$receiver_ip2" cypher_ip.txt | awk -F  ":" '{print $1}')
    local history_cypher_line=$(sed -n ${history_ip}p cypher_ip.txt)
    local history_cypher=$(echo $history_cypher_line | cut -d " " -f2)
    local cypher2=$(md5 -s $history_cypher)
    local real_cypher2=$(echo $cypher2 | cut -d " " -f4)
    if [ "$receiver_cypher2" == "$real_cypher2" ] ; then
    echo "$receiver_ip2" ":" "$message2"
    echo "Type Message: " 
    read messageToSend2
    echo "$my_ip;$real_cypher2;$messageToSend" | nc $receiver_ip2 5001 &
    cat cypher_ip.txt | grep -v "$receiver_ip2" > file_temp.txt
    rm cypher_ip.txt
    touch cypher_ip.txt
    cat file_temp.txt >> cypher_ip.txt
    rm file_temp.txt
    echo "$receiver_ip2 $real_cypher2 $messageToSend2" >> cypher_ip.txt
    else
    echo "security alert"
    cat cypher_ip.txt | grep -v "$receiver_ip2" > file_temp.txt
    rm cypher_ip.txt
    touch cypher_ip.txt
    cat file_temp.txt >> cypher_ip.txt
    rm file_temp.txt
    cat sender_cypher_ip.txt | grep -v "$receiver_ip2" > file_temp.txt
    rm sender_cypher_ip.txt
    touch sender_cypher_ip.txt
    cat file_temp.txt >> sender_cypher_ip.txt
    rm file_temp.txt
    fi

    fi

}

receive_message(){
    nc -lk 5001 | while IFS=, read -a p
        do 
        local message_to_receive=${p[0]}
        local source_ip=$(echo $message_to_receive | cut -d ";" -f1)
        local cypher=$(echo $message_to_receive | cut -d ";" -f2)
        local message=$(echo $message_to_receive | cut -d ";" -f3)
        cat sender_cypher_ip.txt | grep -v "192.168.4.199" > file_temp.txt
        rm sender_cypher_ip.txt
        touch sender_cypher_ip.txt

        cat file_temp.txt >> sender_cypher_ip.txt
        rm file_temp.txt
        echo "$source_ip $cypher $message" >> sender_cypher_ip.txt
        sed '/^\s*$/d' sender_cypher_ip.txt > filetemp2.txt
        rm sender_cypher_ip.txt
        touch sender_cypher_ip.txt
        cat filetemp2.txt >> sender_cypher_ip.txt
        rm filetemp2.txt
    done

}






peopledDiscoversYou &
discoverPeople &
sleep 3
receive_message &
while true; do
    firstMessage 
done

