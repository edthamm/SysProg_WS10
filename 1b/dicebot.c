/*
Dicebot
@author Eduard Thamm 0525087
@brief A simple Bot playing a game of dice.
@detail This bot connects to a hardcoded gameserver and plays a simple game of dice against 2 opponents remembering his own and their scores and reporting them on BYE.
@date 

gameserver: sysprog-dicer.tilab.tuwien.ac.at:9001

*/

int main(void){

return 0;
}



//routines for cleaning up on SIGINT
(void) signal(SIGINT, cleanup);
void cleanup(int nSignal){
//todo clean up
}  
