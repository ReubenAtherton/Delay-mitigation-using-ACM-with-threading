# Delay-mitigation-using-ACM-and-threading (Coursework)

---

Abstract.

This paper will look at how Asynchronous Communication Mechanisms (ACMs) can be used to mitigate the bottleneck effect in a ‘First In First Out’ (FIFO) data pipeline. A game will be created whereby the user controls a character, in this case a fox, displayed by the symbol ‘>’ with the intention of catching a second character, a rabbit displayed by the symbol ‘@’. The game is introduced into the FIFO pipeline, a queue like process that executes programs in a consecutive order. Next a delay is added to the pipeline after the game, in order to cause a deliberate data accumulation resulting in a game lag. These effects are observed and discussed. Finally, an ACM is then implemented into the pipeline, between the game and delay in such a way that reduces the effects of data accumulation, making the game playable at the expense of minor frame loss.

---


### Game only

    ./game

### Game with observable delay
Open second terminal window inside folder destination and create fifo using command:

    mkfifo myfifo
    cat myfifo
    
Next, inside first terminal use the command:

    ./game | tee myfifo | ./delay
  
### Game with delay mitigation using ACM (with fifo created)
Open second terminal window inside folder destination:

    cat myfifo   
Next, inside first terminal window use command:

    ./game | tee myfifo | ./acm | ./delay
