#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define T 10
#define J 11
#define Q 12
#define K 13 
#define A 14
#define MAXINPUT 40/*Defien the maximum input character per line */

struct card{
	int value;
	char suit;
	struct card *next;
};
struct card Black[5],White[5];

int high_card (struct card player[]){
	int highest_value=player[0].value;
	int i;
	for(i=1;i<5;i++){
		if(player[i].value>highest_value){
			highest_value=player[i].value;
		}
	}
	return highest_value;

}

int same_number(struct card player[],int target){
	/*target will indicate whether the fucntion is looking for 3 or 4 card with same value */

	/*the range of card value is 2~14*/
	int value [15];
	int i;
	for(i=0;i<15;i++){
		/*initial it to 0*/
		value[i]=0;
	}
	for (i=0;i<5;i++){
		if(player[i].value==0){
			/*impossible for player to get card with 0 value */
			continue;
		}
		/*counting the occurance of each value*/
		value[player[i].value]++;
		if(value[player[i].value]==target){
			/*pattern found , return the value */
			return player[i].value;
		}
	}
	return 0;
}

int two_pair(struct card player[],bool return_higher_value){
	/*if return_higher_value equal to false , the function will return pair with lower value*/

	int first_pair_value=same_number(player,2); 

	if(first_pair_value>0){
		/*first pair detected*/
		struct card new_player_card[5];
		int i;
		for(i=0;i<5;i++){
			/*removing the pair from player card*/
			if(player[i].value==first_pair_value){
				new_player_card[i].value=0;
				new_player_card[i].suit='0';
			}
			else {
				new_player_card[i].value=player[i].value;
				new_player_card[i].suit=player[i].suit;
			}
		}
		int second_pair_value=same_number(new_player_card,2);
		if(second_pair_value!=0){
			/*two pair are found */

			/*determine which having higer value */
			int higer_value,lower_value;
			if(first_pair_value>second_pair_value){
				higer_value=first_pair_value;
				lower_value=second_pair_value;
			}
			else {
				higer_value=second_pair_value;
				lower_value=first_pair_value;
			}
			
			/*return number according to requirement */

			if(return_higher_value)
				return higer_value;
			else 
				return lower_value;
		}
	}
	return 0;
}
int straight(struct card player[]){
	/*record the previous value of card*/

	/*count the appearance of each value */
	int value[15];
	int i;
	for (i=0;i<15;i++){
		value[i]=0;
	}
	for(i=0;i<5;i++){
		value[player[i].value]++;
	}

	int index=0;
	/*find the start of straight*/
	for (i=0;i<15;i++){
		if(value[i]>0){
			index=i;
			break;
		}
	}
	
	if(index==0|| index+5>15){
		return 0;
	}
	for(i=index;i<index+5;i++){
		/*examine the following five value 
		 * if each of the occur onlt once , straight happen */
		if(value[i]!=1){
			return 0;
		}
	}
	return index+4;
}
int flush (struct card player[]){
	/* 0 represent no flush pattern found */
	char suit=player[0].suit;
	int i;
	for(i=1;i<5;i++){
		if(player[i].suit!=suit)
			/*found defferent suit , flust unvalid*/
			return 0;
	}
	return 1;

}

int full_house(struct card player[]){
	int three_of_kind=same_number(player,3);
	if(three_of_kind>0){
		/*creating a new player card with three of kind value eliminated*/
		struct card new_player_card[5];

		int i;
		for(i=0;i<5;i++){
			if(player[i].value==three_of_kind){
				new_player_card[i].value=0;
				new_player_card[i].suit='0';
			}
			else {
				new_player_card[i].value=player[i].value;
				new_player_card[i].suit=player[i].suit;
			}
		}

		/*finding pair*/
		if(same_number(new_player_card,2)>0){
			return three_of_kind;
		}
	}
	return 0;
}

int straight_flush(struct card player []){
	if(flush(player)>0 && straight(player)>0){
		return high_card(player);
	}
	else {
		return 0;
	}
}

int compute_result(int black_ans, int white_ans){
	/*compute who is the winner */

	/*returned value :
	 * 0 no result
	 * 1 Black win
	 * 2 White win
	 * 3 tie
	 */

	if(black_ans>0||white_ans>0){
		if(black_ans>white_ans){
			return 1;
		}
		else if(white_ans>black_ans){
			return 2;
		}
		else {
			return 3;
		}
	}
	return 0;
}

int compute_high_card(){
	int result[2]={0,0};
	int final_ans;
	/*high card*/
	do{
		result[0]=high_card(Black);
		result[1]=high_card(White);
		final_ans=compute_result(result[0],result[1]);
		if(final_ans==3){
			/*if tie , remove the highest value card from both player hand*/
			int i;
			for(i=0;i<5;i++){
				if(Black[i].value==result[0]){
					Black[i].value=0;
					break;
				}
			}
			for(i=0;i<5;i++){
				if(White[i].value==result[0]){
					White[i].value=0;
					break;
				}
			}
		}
	}while(final_ans==3);
	return final_ans;
}
int war(){
	/*first index for Balck while second for white*/
	int result[2]={0,0};
	int final_ans=0;
	/*returned value :
	 * 1 Black win
	 * 2 White win
	 * 3 tie
	 */

	/*Straight Flush*/
	result[0]=straight_flush(Black);
	result[1]=straight_flush(White);
	final_ans=compute_result(result[0],result[1]);
	if(final_ans>0){
		return final_ans;
	}

	/*Four of kind */
	result[0]=same_number(Black,4);
	result[1]=same_number(White,4);
	final_ans=compute_result(result[0],result[1]);
	if(final_ans>0){
		return final_ans;
	}

	/*Full house */
	result[0]=full_house(Black);
	result[1]=full_house(White);
	final_ans=compute_result(result[0],result[1]);
	if(final_ans>0){
		return final_ans;
	}

	/*Flush */
	result[0]=flush(Black);
	result[1]=flush(White);
	final_ans=compute_result(result[0],result[1]);
	if(final_ans>0){
		if(final_ans==3){
			/*both are tie
			 * start computing for high card*/

			return compute_high_card();
		}
		return final_ans;
	}

	/*Straight */
	result[0]=straight(Black);
	result[1]=straight(White);
	final_ans=compute_result(result[0],result[1]);
	if(final_ans>0){
		return final_ans;
	}

	/*Three of kind */
	result[0]=same_number(Black,3);
	result[1]=same_number(White,3);
	final_ans=compute_result(result[0],result[1]);
	if(final_ans>0){
		return final_ans;
	}

	/*Two pair*/
	int black_pair_value[2],white_pair_value[2];

	/*higher pait value is stored at index 0*/
	black_pair_value[0]=two_pair(Black,true);
	white_pair_value[0]=two_pair(White,true);
	
	/*lower pait value is stored at index 1*/
	black_pair_value[1]=two_pair(Black,false);
	white_pair_value[1]=two_pair(White,false);

	final_ans=compute_result(black_pair_value[0],white_pair_value[0]);
	if(final_ans>0){
		/*two pair is found*/
		if(final_ans==3){
			/*the value of higeher pair is equal 
			 * comput lower pair value*/
			final_ans=compute_result(black_pair_value[1],white_pair_value[1]);

			if(final_ans==3){
				/*pair again 
				 * compute with the remaining card */
				int i;
				for (i=0;i<5;i++){
					int temp_int=Black[i].value;
					if(temp_int!=black_pair_value[0] && temp_int!=black_pair_value[1]){
						result[0]=temp_int;
						break;
					}
				}
				for (i=0;i<5;i++){
					int temp_int=White[i].value;
					if(temp_int!=white_pair_value[0] && temp_int!=white_pair_value[1]){
						result[1]=temp_int;
						break;
					}
				}
				final_ans=compute_result(result[0],result[1]);
				return final_ans;
			}
			return final_ans;
		}
		return final_ans;
	}

	/*pair*/
	result[0]=same_number(Black,2);
	result[1]=same_number(White,2);
	final_ans=compute_result(result[0],result[1]);
	if(final_ans>0){
		if(final_ans==3){
			/*tie 
			 * compute high_card*/

			/*removing pair from both player*/
			int i;
			for (i=0;i<5;i++){	
				if(result[0]==Black[i].value){
					Black[i].value=0;
					break;
				}
			}
			for (i=0;i<5;i++){	
				if(result[0]==White[i].value){
					White[i].value=0;
					break;
				}
			}
			/*high card*/
			return compute_high_card();
		}
		return final_ans;
	}

	/*high card*/
	return compute_high_card();

}


int get_card_value(char input){
	if(input-'0'<10){
		/* if the value of card is within 2~9*/
		return input-'0';
	}
	else {
		switch(input){
			case 'A':
				return A;
			case 'T':
				return T;
			case 'J':
				return J;
			case'Q':
				return Q;
			case'K':
				return K;
			default:
				return 0;
		}
	}
}

int main (){
	char input[MAXINPUT];
	while(fgets(input,sizeof(input),stdin)!=NULL){
		if(input[0]=='\n'){
			continue;
			/*handled nexline */
		}
		char *input_index=&input[0];
		int i;
		for(i=0;i<5;i++){
			/*Read all the card for black player*/

			/*Read the value of card and move to next input char*/
			Black[i].value=get_card_value(*input_index++);
			/*get the suit of the card and move to next char*/
			Black[i].suit=*input_index++;
			Black[i].next=NULL;
			input_index++;/* ignore white space */
		}
		for(i=0;i<5;i++){
			/*Read all the card for White player*/

			/*Read the value of card and move to next input char*/
			White[i].value=get_card_value(*input_index++);
			/*get the suit of the card and move to next char*/
			White[i].suit=*input_index++;
			White[i].next=NULL;
			input_index++;/* ignore white space */
		}
		memset(input,'\0',sizeof(input));
		int result=war();
		if(result==1){
			printf("Black wins.\n");
		}
		else if(result==2){
			printf("White wins.\n");
		}
		else {
			printf("Tie.\n");
		}

	}
}
