require 'rubygems'
require 'mqtt'
require 'httparty'
require 'date'

puts "Connecting client"
client = MQTT::Client.connect(	:host => "broker.hivemq.com", 
								:port => 1883
								)


puts "Subbing"
#client.subscribe( 'onetwotest/devices/+/up' )
client.subscribe( 'MrFire/#' )


	Sub_Logger = Thread.new do
		puts "Waiting for data"
		client.get do |topic,message|
			# Block is executed for every message received
			puts "Received " + '[' + topic + '] ' + message
			File.write('log.log', '[' + Time.now.strftime("%d/%m/%Y %H:%M:%S") + '] ' + '[' + topic + '] ' + message + "\n", mode: 'a')
		end
	end

puts "\nPress enter to kill\n"
	
gets
Sub_Logger.kill