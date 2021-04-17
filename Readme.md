Ring Buffer lib: https://github.com/rlogiacco/CircularBuffer


To change the Operator Mode:
- hang up, pick up, hang up, pick up, dial digit for operator mode to toggle
- modes:
  - 1: Serial (print what number was dialed)
  - 2: NeoPixel
  - 3: SMS Message Content
  - 4: SMS Recipient phone number
  - 5: SMS message manager (send message)
  - 6: Operator mode manager (displays the enabled/disables status for each mode)

Recipient phone number:
- enter 555 to clear the phone number
SMS Message Content:
- enter text in 2 key sequences.
  - The first number is the character group - what key on the phone pad the letter is assigned to. The 2nd key is the position within that group.
  - Example: the 4 key has letters GHI. So to dial 'h', you would neter 42. 4 as the character group key, then 2 because H is the 2nd letter in GHI
  - Group 7 is PQRS, group 9 is WXYZ, all other groups are 3 letters
  - All lowercase. Potential enhancement.
- the 0 group is for deleting text
  - 0 1 deletes the last character
  - 0 0 deletes the entire message
- the 1 group is for non-alpha characters (punctuation)
SMS Manager:
- dial 7 to connect to wifi
- dial 4 to send message to recipient
- uses twilio to send messages