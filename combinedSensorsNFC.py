# This is an example program to show how to generate speech with the Raspberry Pi.
# You can see the full written tutorial here:  http://www.dexterindustries.com/howto/make-your-raspberry-pi-speak/
# This program makes the Pi speak aloud the text entered by the user and it also records it to a file named Text.wav.
# Last edited Talha Rao -July 2022

from num2words import num2words
from subprocess import call  # writing to command line

cmd_beg = "espeak "  # first part of the command argument
# To play back the stored .wav file and to dump the std errors to /dev/null
cmd_end = "  2>/dev/null"
# cmd_out= '--stdout > /home/pi/Desktop/Text.wav ' # To store the voice file
cmd_out = "-ven+m1 -s110 -p10 -g5 "  # To change the voice

file1 = open("TextForAudio.txt", "r")  # Opening txt file
Lines = file1.readlines()  # Reading lines

count = 0  # setting counter to 0
# Strips the newline character
for line in Lines:
    count += 1
    text = line.format(count, line.strip())#"Line{}: {}"
    text = text.replace(" ", "_")  # Replace spaces in the call to _ so the program dosent get confused
    print(cmd_beg + cmd_out + text + cmd_end)  # Printing out the call for debugging purposes
    # Calls the Espeak TTS Engine to read aloud a Text
    call([cmd_beg + cmd_out + text + cmd_end], shell=True)

file1.close()  # Close file to prevent access problems

# Uncomment this to make the program only run onces and end nicely # sys.exit("The python program ended")
# to finish program "ctrl z" then "ps" then kill "kill -9 PID#"
