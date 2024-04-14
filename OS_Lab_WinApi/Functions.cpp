#include "Definitions.h"

void SetTextFromMatrix(int i, char* str, char** strMatr) {
	if (str == nullptr || strMatr == nullptr) return;

	if (i != 0) str[(i - 1) * CHARS_STR_ROW_LEN + CHARS_STR_ROW_LEN - 1] = '\n';
	for (int j = 0; j < GEN_CHARS_STR_ROW_LEN; j++) {
		str[i * CHARS_STR_ROW_LEN + j * 2] = strMatr[i][j];
		if (j != 0) str[i * CHARS_STR_ROW_LEN + j * 2 - 1] = '\t';
	}
	str[i * CHARS_STR_ROW_LEN + CHARS_STR_ROW_LEN - 1] = '\0';
}