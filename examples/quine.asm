LOAD LDA LOAD  // Load position 0 into the accumulator. This line will be modified on each loop to load the next lines into the accumulator
     OUT       // Output the accumulator's value. The accumulator's value will be the line that was just loaded
     SUB ONE   // Subtract 1 from the value in the accumulator. This is so we can do the BRZ in the next step to see if we are on the last line in the program
     BRZ ONE   // If the previous subtraction has made the accumulator 0 (which means we had the value 001 in the accumulator), then branch to position ONE
     LDA LOAD  // Load the LOAD position into the accumulator, this is in preparation to increment the address digits for this position
     ADD ONE   // Increment the position digits for the LOAD line. The value currently in the accumulator would, if read as an instruction, load the next line into the accumulator, compared to the last line loaded
     STA LOAD  // Store the newly incremented LOAD line back in the LOAD position
     BRA LOAD  // Return to the beginning of the loop
ONE  DAT 1     // The variable 1. If read as an instruction, this will be interpreted as HLT/COB and will end the program
