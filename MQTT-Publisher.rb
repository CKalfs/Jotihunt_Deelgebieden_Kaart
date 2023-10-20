require 'rubygems'
require 'mqtt'
require 'httparty'
require 'openssl'	

OpenSSL::SSL::VERIFY_PEER = OpenSSL::SSL::VERIFY_NONE

#set some variables:
select = '2'
nrOfAreas = 6
areaNames = ["Alpha", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot"]
areasCurrent = [0, 0, 0, 0, 0, 0]
$alphaCurrent = 1
$alpha2Current = 0
$bravoCurrent = 0
$bravo2Current = 0
$charlieCurrent = 0
$charlie2Current = 0
$deltaCurrent = 0
$delta2Current = 0
$echoCurrent = 0
$echo2Current = 0
$foxtrotCurrent = 0
$foxtrot2Current = 0
$scoreCurrent = "000"
$scoreGuess = "000"
$opdrachtCurrent = "Unavailable"
$nieuwsCurrent = "Unavailable"
$newData = false

areasLast = [3, 3, 3, 3, 3, 3]
$alphaLast = 3
$alpha2Last = 3
$bravoLast = 3
$bravo2Last = 3
$charlieLast = 3
$charlie2Last = 3
$deltaLast = 3
$delta2Last = 3
$echoLast = 3
$echo2Last = 3
$foxtrotLast = 3
$foxtrot2Last = 3
$scoreLast = "000"
$scoreGuessLast = "000"
$opdrachtLast = "Unavailable"
$nieuwsLast = "Unavailable"

$secondTime = false

#									  _____      _   _                
#									 / ____|    | | | |               
#									| |  __  ___| |_| |_ ___ _ __ ___ 
#									| | |_ |/ _ \ __| __/ _ \ '__/ __|
#									| |__| |  __/ |_| ||  __/ |  \__ \
#									 \_____|\___|\__|\__\___|_|  |___/
#Vossen stuf
def getVossen(areaNamesFunction, areaCurrentFunction)
	
	puts "Getting last 'vossen' update..."
	url = 'https://jotihunt.nl/api/2.0/areas'

	response = HTTParty.get(url)
	vossen_Status = response.body
	
	i = 0
	areaNamesFunction.each do |areaNamesFunction|
		status = vossen_Status[vossen_Status.index("status", vossen_Status.index(areaNamesFunction)) + 9]
		if status == "r"
			areaCurrentFunction = 0
		elsif status == "g"
			areaCurrentFunction = 2
		else
			areaCurrentFunction = 1
		end
		i += 1
	end
	
	#$alphaCurrent = vossen_Status[vossen_Status.index("Alpha") + 17]
	#$bravoCurrent = vossen_Status[vossen_Status.index("Bravo") + 17 .. vossen_Status.index("}", vossen_Status.index("Bravo")) - 2]
	#$charlieCurrent = vossen_Status[vossen_Status.index("Charlie") + 19 .. vossen_Status.index("}", vossen_Status.index("Charlie")) - 2]
	#$deltaCurrent = vossen_Status[vossen_Status.index("Delta") + 17 .. vossen_Status.index("}", vossen_Status.index("Delta")) - 2]
	#$echoCurrent = vossen_Status[vossen_Status.index("Echo") + 16 .. vossen_Status.index("}", vossen_Status.index("Echo")) - 2]
	#$foxtrotCurrent = vossen_Status[vossen_Status.index("Foxtrot") + 19 .. vossen_Status.index("}", vossen_Status.index("Foxtrot")) - 2]
	
	#$alpha2Current = vossen_Status[vossen_Status.index("Alpha 2") + 19 .. vossen_Status.index("}", vossen_Status.index("Alpha 2")) - 2]
	#$bravo2Current = vossen_Status[vossen_Status.index("Bravo 2") + 19 .. vossen_Status.index("}", vossen_Status.index("Bravo 2")) - 2]
	#$charlie2Current = vossen_Status[vossen_Status.index("Charlie 2") + 21 .. vossen_Status.index("}", vossen_Status.index("Charlie 2")) - 2]
	#$delta2Current = vossen_Status[vossen_Status.index("Delta 2") + 19 .. vossen_Status.index("}", vossen_Status.index("Delta 2")) - 2]
	#$echo2Current = vossen_Status[vossen_Status.index("Echo 2") + 18 .. vossen_Status.index("}", vossen_Status.index("Echo 2")) - 2]
	#$foxtrot2Current = vossen_Status[vossen_Status.index("Foxtrot 2") + 21 .. vossen_Status.index("}", vossen_Status.index("Foxtrot 2")) - 2]
	
end

#Opdrachten info
def getOpdracht
	
	puts "Getting last 'opdracht' update... "
	url = 'https://jotihunt.net/api/1.0/opdracht'

	opdracht_Site = HTTParty.get(url)
	opdracht_Status = opdracht_Site.body
	
	$opdrachtCurrent = opdracht_Status[opdracht_Status.index("titel") + 8 .. opdracht_Status.index("datum", opdracht_Status.index("titel")) - 4]
		
	return 
	
end

#Nieuws info
def getNieuws
	
	puts "Getting last 'news' update... "
	url = 'https://jotihunt.net/api/1.0/nieuws'

	nieuws_Site = HTTParty.get(url)
	nieuws_Status = nieuws_Site.body
	
	$nieuwsCurrent = nieuws_Status[nieuws_Status.index("titel") + 8 .. nieuws_Status.index("datum", nieuws_Status.index("titel")) - 4]
		
	return 
	
end


#Score
def getScore

	puts "Getting last 'score' update... "


	url = 'https://jotihunt.net/api/1.0/scorelijst'
	
	
	score_Site = HTTParty.get(url, {headers: {token: 'S614a3bf6b84e00.21843616'}})
	score_Status = score_Site.body
	
	if score_Status.include?("Scouting Groessen")
	
		$scoreCurrent = score_Status[score_Status.index("totaal", score_Status.index("Scouting Groessen")) + 8 .. score_Status.index("}", score_Status.index("Scouting Groessen")) - 1]
	
	end
	
	return 
end

#		  __  __  ____ _______ _______               _     _ _     _                   
#		 |  \/  |/ __ \__   __|__   __|             | |   | (_)   | |                  
#		 | \  / | |  | | | |     | |     _ __  _   _| |__ | |_ ___| |__   ___ _ __ ___ 
#		 | |\/| | |  | | | |     | |    | '_ \| | | | '_ \| | / __| '_ \ / _ \ '__/ __|
#		 | |  | | |__| | | |     | |    | |_) | |_| | |_) | | \__ \ | | |  __/ |  \__ \
#		 |_|  |_|\___\_\ |_|     |_|    | .__/ \__,_|_.__/|_|_|___/_| |_|\___|_|  |___/
#		                                | |                                            
#		                                |_|
#MQTT publisher
def mqttPublish (message)
	MQTT::Client.connect("broker.hivemq.com", 1883) do |c|
		c.publish("MrFire/16Seg/Scroll", message)
	end
end

def mqttPublishStanding(message)
	MQTT::Client.connect("broker.hivemq.com", 1883) do |c|
		c.publish("MrFire/16Seg/Input", message)
	end
end

def mqttClockEnable
	MQTT::Client.connect("broker.hivemq.com", 1883) do |c|
		c.publish("MrFire/16Seg/Clock", "1")
	end
end

def mqttVossenStatus(region, status)
	MQTT::Client.connect("broker.hivemq.com", 1883) do |c|
		c.publish("MrFire/Vossen/" + region, status, retain=true)
	end
end

#"main"
while true do

	api_Downloader = Thread.new {
		while true
						
			#print "[API_Downloader] "
			#getScore
			print "[API_Downloader] "
			puts "Getting last 'vossen' update..."
			url = 'https://jotihunt.nl/api/2.0/areas'

			response = HTTParty.get(url)
			vossen_Status = response.body

			i = 0
			areaNames.each do |areaNames|
				status = vossen_Status[vossen_Status.index("status", vossen_Status.index(areaNames)) + 9]
				if status == "r"
					areasCurrent[i] = 0
				elsif status == "g"
					areasCurrent[i] = 2
				else
					areasCurrent[i] = 1
				end
				i += 1
			end
			#print "[API_Downloader] "
			#getOpdracht
			#print "[API_Downloader] "
			#getNieuws
			$newData = true
			sleep 5
			
		end #Thread 'main'
	} #Thread

	#api_Downloader.start
	
	
	_16Segment = Thread.new do
		lastChange = 0
		clockEnabled = false
		scoreSet = false

		while true
		
			i = 0
			while $newData == false
				puts "[16Segment] Waiting for data"
				if i > 40
					i = 0
				end
				sleep 0.5
				i += 1
			end
			
			case select
			
				when '1'
					
					#Do nothin, let something else control the 16 segment
					puts "[16Segment] Nothing today"
					
					
				when '2'
				
					sleep 1
					
					print "[16Segment] Constructing message"
					message = " "
#					  _____        _                         _   _             
#					 |  __ \      | |                       | | (_)            
#					 | |  | | __ _| |_ __ _   ___  ___  _ __| |_ _ _ __   __ _ 
#					 | |  | |/ _` | __/ _` | / __|/ _ \| '__| __| | '_ \ / _` |
#					 | |__| | (_| | || (_| | \__ \ (_) | |  | |_| | | | | (_| |
#					 |_____/ \__,_|\__\__,_| |___/\___/|_|   \__|_|_| |_|\__, |
#					                                                      __/ |
#					                                                     |___/
					i = 0
					while i < nrOfAreas
						if areasCurrent[i] != areasLast[i]
							mqttVossenStatus(areaNames[i], areasCurrent[i])
							mqttVossenStatus(areaNames[i] + "2", areasCurrent[i])
							
							message = message + areaNames[i] + " is "
							if areasCurrent[i] == 0
								message = message + "rood "
							elsif areasCurrent[i] == 1
								message = message + "oranje "
							elsif areasCurrent[i] == 2
								message = message + "groen "
							end
							areasLast[i] = areasCurrent[i]
							puts "."
						end
						i += 1
					end
					
				
					if ($scoreCurrent != "000") && (scoreCurrent != $scoreLast) 
					
						message = message + "ONZE SCORE IS NU: " + $scoreCurrent + "   "
						$scoreLast = $scoreCurrent
						print "."
						
					elsif ($scoreGuess != "000") && ($scoreGuess != $scoreGuessLast)
					
						message = message + "ONZE SCORE IS WAARSCHIJNLIJK: " + $scoreGuess + "   "
						$scoreGuessLast = $scoreGuess
						print "."
					
					end
					
					if $opdrachtCurrent != $opdrachtLast
						
						message = message + "NIEUWE OPDRACHT:" + $opdrachtCurrent + "   "
						$opdrachtLast = $opdrachtCurrent
						print "."
						
					end
					
					if $nieuwsCurrent != $nieuwsLast
					
						message = message + "NIEUWS:" + $nieuwsCurrent + "   "
						$nieuwsLast = $nieuwsCurrent
						print "."
						
					end
					
					
#			  __  __                                     _____                _ _             
#			 |  \/  |                                   / ____|              | (_)            
#			 | \  / | ___  ___ ___  __ _  __ _  ___    | (___   ___ _ __   __| |_ _ __   __ _ 
#			 | |\/| |/ _ \/ __/ __|/ _` |/ _` |/ _ \    \___ \ / _ \ '_ \ / _` | | '_ \ / _` |
#			 | |  | |  __/\__ \__ \ (_| | (_| |  __/    ____) |  __/ | | | (_| | | | | | (_| |
#			 |_|  |_|\___||___/___/\__,_|\__, |\___|   |_____/ \___|_| |_|\__,_|_|_| |_|\__, |
#										  __/ |                                          __/ |
#										 |___/                                          |___/

					puts #creating a newline
					
					if message.length >= 2
						
						puts "[16Segment] Message is: " + message
						mqttPublish(message)
						lastChange = 0
						clockEnabled = false
						scoreSet = false
						
					elsif lastChange >= 5
					
						if clockEnabled == false
							puts "[16Segment] No messages for a long time, enabling Clock"
							mqttClockEnable
							clockEnabled = true
						end
					
					elsif lastChange >= 2

						if scoreSet == false
							puts "[16Segment] No new messages, putting standing score"
							mqttPublishStanding("SCORE:" + $scoreLast)
							scoreSet = true
						end
						
					end
					
					lastChange += 1
					puts lastChange
					
			end #case
			puts "[16Segment] Done & waiting"
			$newData = false
			sleep 5
			
		end #Thread 'main'
	end #Thread
	
	while true
	
		puts "\nPress enter to enter menu\n"
		
		gets
		
		
		
		print 	"Choose what to send:\n" +
				"0. Exit\n" +
				"1. Nothing\n" +
				"2. Add score to guess\n" +
				"Choice: "
		
		select = gets.chomp
	
		if select == '0'
			api_Downloader.kill
			_16Segment.kill
			exit
		
		elsif select == '2'
		
			print 	"Current score is " + $scoreGuess + "\n" +
					"Enter score to add to the current: "
		
			score = gets.chomp
			tempInt = Integer($scoreGuess) + Integer(score)
			$scoreGuess = tempInt.to_s
			
		end
	
	end

end



