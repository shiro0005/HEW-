#include "number.h"


void Score_Draw(float x, float y, int score, int fig, bool bZero)
{
	// �J���X�g�p�̍ő吔�l�����
	int count_stop_score = 1;
	for( int i = 0; i < fig; i++ ) {
		count_stop_score *= 10;
	}

	//�ő�l�̕␳����
	if( score >= count_stop_score ) {
		score = count_stop_score - 1;
	}

//	if( bZero ) {

		for( int i = 0; i < fig; i++ ) { 
			
			int n = score % 10;
			score /= 10;

			Number_Draw(x + NUMBER_WIDTH * (fig - (i + 1)), y, n);
		}
//	}
}
