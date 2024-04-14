#include "Definitions.h"

char* (_cdecl* generateCharSequence)(int size);
void (_cdecl* sortCharSequence)(char* str);

char** strMatr_Ref;
HWND DefaultCharStrTextBox_Ref = NULL;
HWND SortedCharStrTextBox_Ref = NULL;

std::atomic<bool> isSubThreding = false;

void sorterDelegate(std::mutex& mtx, std::condition_variable& cv, std::atomic<bool>& isCycleCompleted);
void generatorDelegate(std::mutex& mtx, std::condition_variable& cv, std::atomic<bool>& isCycleCompleted);

void generatorDelegate(std::mutex& mtx, std::condition_variable& cv, std::atomic<bool>& isCycleCompleted) {
	int i = 0;
	char* str = new char[CHARS_STR_COL_LEN * CHARS_STR_ROW_LEN];

	while (i < CHARS_STR_COL_LEN) {
		std::unique_lock<std::mutex> lock(mtx);

		if (!isSubThreding) return;

		while (!isCycleCompleted) {
			cv.wait(lock);
		}

		strMatr_Ref[i] = generateCharSequence(GEN_CHARS_STR_ROW_LEN);
		Sleep(500);
		SetTextFromMatrix(i, str, strMatr_Ref);
		SetWindowTextA(DefaultCharStrTextBox_Ref, str);

		++i;

		isCycleCompleted = false;
		cv.notify_one();
	}
}
void sorterDelegate(std::mutex& mtx, std::condition_variable& cv, std::atomic<bool>& isCycleCompleted) {
	int i = 0;
	char* str = new char[CHARS_STR_COL_LEN * CHARS_STR_ROW_LEN];

	while (i < CHARS_STR_COL_LEN) {
		std::unique_lock<std::mutex> lock(mtx);
		
		if (!isSubThreding) return;

		while (isCycleCompleted) {
			cv.wait(lock);
		}

		sortCharSequence(strMatr_Ref[i]);
		SetTextFromMatrix(i, str, strMatr_Ref);
		SetWindowTextA(SortedCharStrTextBox_Ref, str);

		++i;

		isCycleCompleted = true;
		cv.notify_one();
	}
}
int ImportLibrary(HMODULE& DllModule) {
	DllModule = LoadLibrary(_T("OS_Lab_Dll.dll"));
	if (DllModule == nullptr) {
		MessageBox(NULL, _T("Cannot open file."), L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}
	generateCharSequence = (char* (*)(int))GetProcAddress(DllModule, "generateCharSequence");
	sortCharSequence = (void (*)(char*))GetProcAddress(DllModule, "sortCharSequence");
	if (generateCharSequence == nullptr) {
		FreeLibrary(DllModule);
		MessageBox(NULL, _T("Procedure generateCharSequence is undefined."), L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}
	if (sortCharSequence == nullptr) {
		FreeLibrary(DllModule);
		MessageBox(NULL, _T("Procedure sortCharSequence is undefined."), L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}
}
void CloseGeneratorThread() {
	isSubThreding = false;
}

DWORD WINAPI SubThreadDelegate(LPVOID params) {
	isSubThreding = true;

	HMODULE DllModule;
	ImportLibrary(DllModule);

	strMatr_Ref = ((GeneratorParams*)params)->strMatr;
	SortedCharStrTextBox_Ref = ((GeneratorParams*)params)->fields.sortedText;
	DefaultCharStrTextBox_Ref = ((GeneratorParams*)params)->fields.defaultText;

	std::mutex mtx;
	std::atomic<bool> isCycleCompleted = true;
	std::condition_variable cv;

	std::thread generator = std::thread(generatorDelegate, std::ref(mtx), std::ref(cv), std::ref(isCycleCompleted));
	std::thread sorter = std::thread(sorterDelegate, std::ref(mtx), std::ref(cv), std::ref(isCycleCompleted));
	generator.join();
	sorter.join();

	FreeLibrary(DllModule);
	CloseSubThread();
	return 0;
}


