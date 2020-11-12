#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

#include <ac_int.h>
#include <ac_channel.h>

typedef ac_int<4,false> Card;

class BlackJack {
	private:
		int sum_card;
		int count_card;
		bool ace_found;
	public:
	BlackJack(){
		sum_card = 0;
		count_card = 0;
		ace_found = false;
	}
	
	void run (ac_channel<Card> &in_card, bool &end_round, bool &win){
		
		if (win){
			end_round = true;
		}
		else{
			if (in_card.available(1)){
				Card next_card = in_card.read();
				count_card += 1;
				
				if (count_card==6){end_round=true;}
				else{
					if (next_card == 1){
						if (ace_found) {win = true;}
						else {
							ace_found = true;
							sum_card += 11;
						}
					}
					
					else {
						sum_card += next_card;
					}
					
					if (sum_card>20){
						end_round = true;
						if (sum_card==21){win=true;}
					}
				}
				run(in_card,end_round,win);
			}
		}
	}
	
	void reset(){
		sum_card = 0;
		count_card = 0;
		ace_found = 0;
	}
};

int main(){
	ac_channel<Card> test_hands [9];
	Card test_in_card;
	
	//Test 1: No Cards - - should not end - should not win
	
	//Test 2: Sum 20 - should not end - should not win
	
	test_in_card = 1;
	test_hands[1].write(test_in_card);
	test_in_card = 19;
	test_hands[1].write(test_in_card);
	
	//Test 3: Sum 24 - should end - should not win
	
	test_in_card = 10;
	test_hands[2].write(test_in_card);
	test_in_card = 8;
	test_hands[2].write(test_in_card);
	test_in_card = 6;
	test_hands[2].write(test_in_card);
	
	//Test 4: Sum 21 - should end - should win
	
	test_in_card = 9;
	test_hands[3].write(test_in_card);
	test_in_card = 7;
	test_hands[3].write(test_in_card);
	test_in_card = 5;
	test_hands[3].write(test_in_card);
	
	//Test 5: Sum 21 but Card Count > 5 - should end - should not win
	
	test_in_card = 1;
	test_hands[4].write(test_in_card);
	test_in_card = 2;
	test_hands[4].write(test_in_card);
	test_in_card = 3;
	test_hands[4].write(test_in_card);
	test_in_card = 4;
	test_hands[4].write(test_in_card);
	test_in_card = 5;
	test_hands[4].write(test_in_card);
	test_in_card = 6;
	test_hands[4].write(test_in_card);
	
	// Test 6: Sum > 21 and Card Count > 5 but has 2 aces before - should end - should win
	
	test_in_card = 4;
	test_hands[5].write(test_in_card);
	test_in_card = 1;
	test_hands[5].write(test_in_card);
	test_in_card = 3;
	test_hands[5].write(test_in_card);
	test_in_card = 1;
	test_hands[5].write(test_in_card);
	test_in_card = 5;
	test_hands[5].write(test_in_card);
	test_in_card = 10;
	test_hands[5].write(test_in_card);
	
	// Test 7: Sum > 21 and Card Count > 5 - should end - should not win
	
	test_in_card = 4;
	test_hands[6].write(test_in_card);
	test_in_card = 3;
	test_hands[6].write(test_in_card);
	test_in_card = 3;
	test_hands[6].write(test_in_card);
	test_in_card = 7;
	test_hands[6].write(test_in_card);
	test_in_card = 5;
	test_hands[6].write(test_in_card);
	test_in_card = 10;
	test_hands[6].write(test_in_card);
	
	// Test 8: Card Count > 5 but Sum == 21 before - should end - should win
	
	test_in_card = 4;
	test_hands[7].write(test_in_card);
	test_in_card = 1;
	test_hands[7].write(test_in_card);
	test_in_card = 3;
	test_hands[7].write(test_in_card);
	test_in_card = 1;
	test_hands[7].write(test_in_card);
	test_in_card = 5;
	test_hands[7].write(test_in_card);
	test_in_card = 10;
	test_hands[7].write(test_in_card);
	
	// Test 9: Sum < 21 and 2 aces - should end - should win
	
	test_in_card = 3;
	test_hands[8].write(test_in_card);
	test_in_card = 1;
	test_hands[8].write(test_in_card);
	test_in_card = 4;
	test_hands[8].write(test_in_card);
	test_in_card = 1;
	test_hands[8].write(test_in_card);
	test_in_card = 2;
	
	BlackJack controller;
	
	for (int i=0; i<9;i++){
		bool test_end_round = false;
		bool test_won = false;
		
		controller.run(test_hands[i], test_end_round, test_won);
		
		std::cout <<"Test #"<< i+1 << ":  ||  Round Ended: [" << test_end_round << "]  Round Won: [" << test_won << "]\n";
		
		controller.reset();
		
	}
	
	return 0;
}
