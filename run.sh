
for prob in 0.005 `seq 0.01 0.01 0.05` 
do	
	
	for runs in `seq 1 5 100`
	do

		rm NetDevice-*; rm Consumer-*;
		export NS_LOG=;
		./waf --run "$1 --prob=$prob --runNumber=$runs";

		for i in $(ls NetDevice-*-IorD.txt)
		do
			echo -n $i > /dev/tty
			echo -n "  ";
			echo `wc -l $i | cut -d ' ' -f1 -`
			
		done

		for i in $(ls Consumer-*-Delay.csv)
		do
			delay=$((`sed -rn 's/(Average delay : )([0-9]+)/\2/p' <$i`))
			echo -n $i > /dev/tty
			echo -n "  ";
			echo $delay;
		done

		totalRetransmissions=0;
		for i in $(ls Consumer-*-Retransmissions.txt)
		do
			retransmissions=$((`sed -rn 's/(Total number of retransmissions are : )([0-9]+)/\2/p' <$i`))
			echo -n $i > /dev/tty
			echo -n "  ";
			echo $retransmissions;
			totalRetransmissions=$(($totalRetransmissions+$retransmissions));
		done

		totalLosses=$((`cat NetDevice-*-ReceiverDrops*.txt | tr [:space:] '\n' | grep -v "^\s*$" | sort | uniq -c | sort -bnr | wc -l`))
		echo -n 'Total Link Layer Losses  ' > /dev/tty
		echo $totalLosses

		if [ "$2" -gt "0" ]
		then
			linkLosses=0;
			for i in $(ls NetDevice-*-ReceiverDrops*.txt) 
			do
				linkLosses=$(($linkLosses + `cat $i | tr [:space:] '\n' | grep -v "^\s*$" | sort | uniq -c | sort -bnr | wc -l`));
				for j in `seq 1 1 $2`
				do
					linkLosses=$(($linkLosses - `cat $i | tr [:space:] '\n' | grep -v "^\s*$" | sort | uniq -c | sort -bnr | grep "$j " | wc -l`));	
				done
				echo -n $i > /dev/tty
				echo -n '  '		
				echo `wc -l $i | cut -d ' ' -f1 -`
			done
			
			echo -n 'Unrecovered Link Layer Losses  ' > /dev/tty
			echo $linkLosses
			
			for i in $(ls NetDevice-*-DetectedTime.txt)
			do
				delay=$((`sed -rn 's/(Average detection time = )([0-9]+)/\2/p' <$i`))
				echo -n $i > /dev/tty
				echo -n "  ";	
				echo $delay;
			done

			
			for i in $(ls NetDevice-*-Retransmissions.txt)
			do
				echo -n $i > /dev/tty
				echo -n "  ";
				echo `wc -l $i | cut -d ' ' -f1 -`
			done

			
			for i in $(ls NetDevice-*-SenderDrops.txt)
			do
				echo -n $i > /dev/tty
				echo -n "  ";	
				echo `wc -l $i | cut -d ' ' -f1 -`
			done

			for i in $(ls NetDevice-*-PureAcks.txt)
			do
				echo -n $i > /dev/tty
				echo -n "  ";
				echo `wc -l $i | cut -d ' ' -f1 -`
			done

			for i in $(ls NetDevice-*-BufferSize.txt)
			do
				maxBufferSize=$((`sed -rn 's/(Max Packet Size = )([0-9]+)/\2/p' <$i`))
				echo -n $i > /dev/tty
				echo -n "  ";
				echo $maxBufferSize;
			done
			
			for i in $(ls NetDevice-*-Received*.txt)
			do
				echo -n $i > /dev/tty
				echo -n "  ";
				echo `wc -l $i | cut -d ' ' -f1 -`
			done

			for i in $(ls NetDevice-*-Forwarded*.txt)
			do
				echo -n $i > /dev/tty
				echo -n "  ";
				echo `wc -l $i | cut -d ' ' -f1 -`
			done

			for i in $(ls NetDevice-*-TotalAcks.txt)
			do
				echo -n $i > /dev/tty
				echo -n "  ";
				echo `wc -l $i | cut -d ' ' -f1 -`
			done

			for i in $(ls NetDevice-*-ReceiverAckDrops.txt)
			do
				echo -n $i > /dev/tty
				echo -n "  ";
				echo `wc -l $i | cut -d ' ' -f1 -`
			done
		fi	
		echo -e "\n"
		rm NetDevice-*;
		rm Consumer-*;
	done

	echo -e "\n\n\n"
done
