#include "System.h"

//Main Func
void Init();
void Update();
void Render();
void Release();

void TitleInit();
void TitleUpdate();
void TitleRender();

void SelectInit();
void SelectUpdate();
void SelectRender();

void NoteLoadUpdate();
void NoteLoadRender();

void InGameInit();
void InGameSoundInit();
void InGameUpdate();
void InGameRender();
void InGameRelease();

void ResultInit();
void ResultUpdate();
void ResultRender();

bool isbSystemOn = true;				//게임 On, Off
bool isCustomMode = false;				//일반 모드 , 커스텀 맵 제작 모드

//Main Func
//Scene
enum Scene
{
	TitleScene,
	SelectScene,
	NoteLoadScene,
	GameScene,
	ResultScene,
	IntroduceScene,
};
Scene SceneState = TitleScene;
//Scene
//Note Func
void NoteInit();
void NoteUpdate();
void NoteRender();
void NoteRelease();
//NoteFunc
// Introduce Func
void IntroduceUpdate();
void IntroduceRender();

//Introduce Func
//MapFunc
void MapInit();
void MapUpdate();
void MapRender();
void MapRelease();
//MapFunc

//SoundFunc
void SoundInit();
void TitleSoundUpdate();
void SoundUpdate();
void SoundTextRender();
void SoundRelease();

//SoundFunc

//Audio FFT
float VisualizerFilter = 0.06f;
float freqVal = 0.0f;
//Audio FFT
//Audio SET
float EffectSoundVolume = 0.3f;		//효과음 볼륨
float volume = 1.0f;			//메인 음악 볼륨
float pitch = 1.0f;				//음악 음정
float Audi = 0.0f;
//Audio SET
//Note Variable
int NoteSpeed = 1;				//노트가 내려오는 스피드
int NoteCount = 0;				//노트 개수


int NoteSync = 2062;		//노트 싱크

clock_t clockOldInputNote = 0;		//현재 입력한 노트라인의 일정시간값
clock_t clockCurInputNote;			//현재 입력한 노트라인의 일정시간값
clock_t clockAccumulate;			//시간 누적

//Note Variable

//Audio Variable
unsigned int CurPlaySoundProcesstime = 0;			//음악의 재생 진행도
unsigned int SoundLength = 0;						//음악의 총 재생시간
int GenerationCycle = 100;					//노트의 생성주기
clock_t OldSearchTime = 0;			//노트의 생성주기 시간
clock_t CurSearchTime;				//노트의 생성주기 시간
//Audio Variable
//FMOD Variable
FMOD_SYSTEM * g_system;
//FMOD_SOUND *sound;
FMOD_CHANNEL * channel;
FMOD_CHANNEL * EffectSoundChannel;			//효과음 채널
vector<FMOD_SOUND *> sound;				//메인 음악 리스트
FMOD_SOUND * hitSound;					//노트 비트음


FMOD_DSP * dsp;			//음악에 따른 노트를 생성할 수 있도록 해주는 변수
FMOD_DSP_PARAMETER_FFT *fft;			//음악에 따른 노트를 생성할 수 있도록 해주는 변수


FMOD_RESULT result;

int SoundCount = 0;						//읽어들인 노래의 개수
int SoundKeyNum = 0;					//재생할 노래 키값
int Balance = 0;					//노트 출력 값
int OutPutNodeFilter = 0;			//노트 출력 값
//FMOD Variable
//FMOD Func
FMOD_DSP_PARAMETER_FFT* GetFFTPARAMETER(FMOD_SOUND *sound);			//음악 출력 (고속 푸리에 변환에 따른 노트 출력)
const char *  SoundList[] =
{
	"Tutorial",
	"100 Bills",
	"Alan Walker - Fade",
	"DayFox - Departure",
	"Ikson - Wanna",
	"LiQWYD - Morning",
	"Tobu-Colors",
	"The Promised Neverland Op"
};//음악 리스트
typedef struct _SOUNDFILE
{
	char SoundFileName[256];
	bool isExistSoundNoteMap;
}SOUNDFILE;
vector<SOUNDFILE> SoundFileList;
enum InputNotePoint
{
	None,
	Line1_I,
	Line2_I,
	Line3_I,
	Line4_I
};//인 게임 씬에서 입력받은 키의 형태
InputNotePoint CurInputNoteKey = None;
enum NotePoint
{
	Line1,
	Line2,
	Line3,
	Line4
};//노트가 출력될 라인


char LongNoteNode[5][11] =
{
	"■■■■■",
	"■■■■■",
	"■■■■■",
	"■■■■■",
	"■■■■■"
};
char NoteNode[] = "■■■■■";			//노트 형태
typedef struct _NOTE
{
	int NoteKey;
	int nX;
	int nY;
	NotePoint point;
	clock_t CurTime;
	clock_t OldTime;
}NOTE;//노트 구조
void CheckNote();			//스펙트럼(Visualizer)에 따른 노트출력
void UpLoadNode(NotePoint point);			//노트 생성
void UpLoadNode(int NodeCount);				//노트생성

void DropNode(NotePoint point, int NoteKeyCount);			//노트생성
int isNoteExistLinePoint(NOTE note);				//노트가 판정선에 닿았는가
void NoteClick(vector<NOTE> NoteList, NotePoint point);		//해당 라인의 노트 판별 결과 , 해당라인을 눌렀을때 (D,F,J,K) 실행되는 함수
bool DeleteNote(NOTE DelNote);					//노트삭제

vector<NOTE> NoteList;					//화면에 출력된 노트 리스트

vector<NOTE> GetNoteList() { return NoteList; }
//NoteFunc
//Map Variable
const int MapXPoint = 64;			//맵의 X축의 가장 끝부분
const int MapYPoint = 0;			//맵의 Y축의 가장 첫부분
const int MapYEndPoint = 50;		//맵의 Y의 가장 끝부분

const int MainDecision = 45;		//중앙라인의 Y좌표
const int SubDecision1 = 44;		//앞 라인의 Y좌표
const int SubDecision2 = 46;		//뒷 라인의 Y좌표
//const char * DestLine = "├──────┼───────┼───────┼───────┤";
const char * SubDestLine = "◈◈◈◈◈◈◈";		// 앞, 뒷 라인의 판정선
const char * DestLine = "◈※※※※※◈";		//중앙 라인의 판정선



constexpr int DefLine1 = 4;		//Line1의 X좌표
constexpr int DefLine2 = 20;	//Line2의 X좌표
constexpr int DefLine3 = 36;	//Line3의 X좌표
constexpr int DefLine4 = 52;	//Line4의 X좌표
//MapVariable
//Game Variable

int NumberSeq = 0;				//Title씬의 선택변수
int SelectNum = 0;				//Select 씬의 선택변수

//Score
int Combo = 0;
static int Score = 0;			//점수
int ScoreToColor = 0;			//점수에 따른 색깔 부여
int ScorePrintTime = 5000;
clock_t OldScoreToColor = 0, CurScoreToColor = 0;
void ScoreRender();				//화면에 점수 출력

//Score

//Game Variable
//File Variable & Func
FILE *fp;
typedef struct _LOADNOTE
{
	unsigned int NoteTime;
	unsigned int NoteSyncTime;
	short NoteLine;
	int Speed;
	float Pitch;
}LOADNOTE;
vector<_LOADNOTE> NoteTimeData;
void SaveNote(int NoteDestTime , NotePoint point);
bool LoadNote(const char * LoadMusicFileName);
//File Variable & Func
//Time Variable & Func
int curTimeSec;		//Audio Timer
int MaxTimeSec;		//Audio Timer
int curTimeMin;		//Audio Timer
int MaxTimeMin;		//Audio Timer

void GetTransformTimeString(unsigned int time, int * Sec, int * Min);
int GetnTransformTime(unsigned int time);
//Time Variable & Func
int main()
{
	Init();

	DWORD CurTime = 0, OldTime = 0;
	while (isbSystemOn)
	{

		CurTime = GetTickCount();
		if (CurTime - OldTime > 33)
		{
			OldTime = CurTime;
			Update();
			Render();
		}
		//Sleep(33);
	}


	Release();
	return 0;
}
void Init()
{
	InGameInit();
	TitleInit();
	SelectInit();
}
void Update()
{
	switch (SceneState)
	{
	case TitleScene:
		TitleUpdate();
		break;
	case SelectScene:
		SelectUpdate();
		break;
	case NoteLoadScene:
		NoteLoadUpdate();
		break;
	case GameScene:
		InGameUpdate();
		break;
	case ResultScene:
		ResultUpdate();
		break;
	case IntroduceScene:
		IntroduceUpdate();
		break;
	default:
		break;
	}
	

	

}
void Render()
{
	ScreenClear();
	switch (SceneState)
	{
	case TitleScene:
		TitleRender();
		break;
	case SelectScene:
		SelectRender();
		break;
	case NoteLoadScene:
		NoteLoadRender();
		break;
	case GameScene:
		InGameRender();
		break;
	case ResultScene:
		ResultRender();
		break;
	case IntroduceScene:
		IntroduceRender();
		break;
	default:
		break;
	}

	ScreenFlipping();
}
void Release()
{
	InGameRelease();
}
void TitleInit()
{
	TitleSoundUpdate();
}
void TitleUpdate()
{
	int nKey;
	int nKeyArrow;
	if (_kbhit())
	{
		nKey = _getch();
		if (nKey == 'q')
		{
			isbSystemOn = false;
		}
		if (nKey == 'i' || nKey == 'I')
		{
			SceneState = Scene::IntroduceScene;
		}
		if (nKey == 224)
		{
			nKeyArrow = _getch();
			switch (nKeyArrow)
			{
			case 72:		//↑
				FMOD_Channel_Stop(channel);
				--NumberSeq;
				if (NumberSeq < 0)
					NumberSeq = 0;
				break;
			case 80:		//↓
				FMOD_Channel_Stop(channel);
				++NumberSeq;
				if (SoundCount - 1< NumberSeq)
					NumberSeq = SoundCount - 1;
				break;
			}
		}
		TitleSoundUpdate();



		switch (nKey)
		{
		case 13:		//엔터키
			SceneState = Scene::SelectScene;
			break;
		}
		
	}
	
}
void TitleSoundUpdate()
{
	FMOD_System_PlaySound(g_system, sound[NumberSeq], 0, 0, &channel);
}
void TitleRender()
{
	char str[20];
	sprintf(str, ">노래 리스트< : %d", NumberSeq);
	ScreenPrint(35, 0, str);

	for (int i = 0; i < SoundCount; i++)
	{
		if (i == NumberSeq)
		{
			SetColor(9);
			ScreenPrint(35, i + 1, SoundList[i]);
			SetColor(15);
		}
		else
		{
			
			ScreenPrint(35, i + 1, SoundList[i]);
		}
	}

	ScreenPrint(35, SoundCount + 1, "i : 도움말");
	ScreenPrint(35, SoundCount + 2, "q : 게임종료");
}
void SelectInit()
{
	SelectNum = 0;
}
void SelectUpdate()		//선택 씬 업데이트
{
	int nKey;
	int nKeyArrow;
	if (_kbhit())
	{
		nKey = _getch();
		if (nKey == 'q')
		{
			isbSystemOn = false;
		}
		if (nKey == 224)
		{
			nKeyArrow = _getch();
			switch (nKeyArrow)
			{
			case 72:		//↑
				--SelectNum;
				if (SelectNum < 0)
					SelectNum = 0;
				break;
			case 80:		//↓
				++SelectNum;
				if (2 < SelectNum)
					SelectNum = SelectNum - 1;
				break;
			}
		}
		switch (nKey)
		{
		case 13:		//엔터키
			FMOD_Channel_Stop(channel);
			if (SelectNum == 0)
			{
				SceneState = Scene::NoteLoadScene;
				SoundKeyNum = NumberSeq;
			}
			else if (SelectNum == 1)
			{
				SceneState = Scene::GameScene;
				SoundKeyNum = NumberSeq;
			}
			/*else if (SelectNum == 2)
			{
				TitleInit();
				SceneState = Scene::TitleScene;
			}*/

			isCustomMode = (SelectNum == 0) ? false : true;		//일반 플레이모드는 커스텀모드를 해제한다.
			if (SceneState == Scene::GameScene)
			{
				SoundKeyNum = NumberSeq;
				InGameSoundInit();
			}
			break;
		}

	}

}
void SelectRender()
{
	char str[256];
	sprintf(str, ">%s< : %d", SoundList[NumberSeq], SelectNum);
	ScreenPrint(0, 0, str);


	ScreenPrint(0, 1, "1.일반 플레이");
	ScreenPrint(0, 2, "2.커스텀 자동 노트 맵 제작");
	//ScreenPrint(0, 3, "3.뒤로가기");
	if (SelectNum == 0)
	{
		SetColor(9);
		ScreenPrint(0, 1, "1.일반 플레이");
	}
	else if (SelectNum == 1)
	{
		SetColor(9);
		ScreenPrint(0, 2, "2.커스텀 자동 노트 맵 제작");
	}
	/*else if (SelectNum == 2)
	{
		SetColor(9);
		ScreenPrint(0, 3, "3.뒤로가기");
	}*/
	SetColor(15);
}

bool isNoteLoading = true;
void NoteLoadUpdate()
{
	if (LoadNote(SoundList[SoundKeyNum]))		//로딩이 끝났을때 -> 인게임씬으로 넘어감(넘어간 씬은 일반 플레이 (커스텀 X))
	{
		SceneState = Scene::GameScene;
		if (SceneState == Scene::GameScene)
		{
			InGameSoundInit();
		}
		isNoteLoading = false;
	}
	else
	{
		isNoteLoading = true;
	}
}
void NoteLoadRender()
{
	if (isNoteLoading)		//로딩중일때
	{
		ScreenPrint(0, 0, "로딩중...");
	}
	else
	{
		ScreenPrint(0, NoteTimeData.size() + 1, "로딩완료!");
	}
}
void InGameInit()
{
	system("mode con cols=100 lines=60");
	srand((unsigned int)time(NULL));
	SoundInit();
	ScreenInit();
	NoteInit();
	MapInit();

}

void InGameSoundInit()
{
	Score = 0;				//스코어 초기화
	GetFFTPARAMETER(sound[SoundKeyNum]);			//노래출력
}

void InGameUpdate()
{
	int nKey = 0;
	ScoreToColor = 0;
	if (isCustomMode)		//커스텀 모드 조작키
	{

		if (_kbhit())
		{

			nKey = _getch();



			if (nKey == 'q')
			{
				isbSystemOn = false;
			}

			switch (nKey)
			{
			case 'D':
			case 'd':
				NoteClick(GetNoteList(), Line1);
				CurInputNoteKey = InputNotePoint::Line1_I;
				/*DropNode(Line1);*/
				break;
			case 'F':
			case 'f':
				NoteClick(GetNoteList(), Line2);
				CurInputNoteKey = InputNotePoint::Line2_I;
				/*DropNode(Line2);*/
				break;
			case 'j':
			case 'J':
				NoteClick(GetNoteList(), Line3);
				CurInputNoteKey = InputNotePoint::Line3_I;
				//DropNode(Line3);
				break;
			case 'K':
			case 'k':
				NoteClick(GetNoteList(), Line4);
				CurInputNoteKey = InputNotePoint::Line4_I;
				//DropNode(Line4);
				break;
			case 'C':
			case 'c':
				VisualizerFilter -= 0.01f;
				if (VisualizerFilter < 0.0f)
					VisualizerFilter = 0.0f;
				break;
			case 'B':
			case 'b':
				VisualizerFilter += 0.01f;
				if (VisualizerFilter > 1.0f)
					VisualizerFilter = 1.0f;
				break;
			case 'l':
				OutPutNodeFilter -= 1;
				if (OutPutNodeFilter < 0)
					OutPutNodeFilter = 0;
				break;
			case 'p':
				OutPutNodeFilter += 1;
				if (OutPutNodeFilter > 10)
					OutPutNodeFilter = 10;
				break;

			case 'm':
				GenerationCycle += 100;
				if (GenerationCycle > 3000)
					GenerationCycle = 3000;
				break;
			case 'n':
				GenerationCycle -= 100;
				if (GenerationCycle < 100)
					GenerationCycle = 100;
				break;
			case '+':
				volume += 0.1f;
				if (volume > 1.0f)
					volume = 1.0f;
				FMOD_Channel_SetVolume(channel, volume);
				break;
			case '-':
				volume -= 0.1f;
				if (volume < 0.0f)
					volume = 0.0f;
				FMOD_Channel_SetVolume(channel, volume);
				break;
			case 'U':
			case 'u':
				pitch += 0.1f;
				if (pitch > 2.0f)
					pitch = 2.0f;
				FMOD_Channel_SetPitch(channel, pitch);

				FMOD_Channel_GetPitch(channel, &pitch);

				break;
			case 'I':
			case 'i':
				pitch -= 0.1f;
				if (pitch < 0.0f)
					pitch = 0.0f;
				FMOD_Channel_SetPitch(channel, pitch);

				FMOD_Channel_GetPitch(channel, &pitch);
				break;

			case '[':
				NoteSpeed -= 1;
				if (NoteSpeed < 0)
					NoteSpeed = 0;
				break;
			case ']':
				NoteSpeed += 1;
				if (NoteSpeed > 10)
					NoteSpeed = 10;
				break;
			}
		}
		
	}
	else    //플레이 모드 조작키
	{
		if (_kbhit())
		{

			nKey = _getch();



			if (nKey == 'q')
			{
				isbSystemOn = false;
			}

			switch (nKey)
			{
			case 'D':
			case 'd':
				NoteClick(GetNoteList(), Line1);
				CurInputNoteKey = InputNotePoint::Line1_I;
				/*DropNode(Line1);*/
				break;
			case 'F':
			case 'f':
				NoteClick(GetNoteList(), Line2);
				CurInputNoteKey = InputNotePoint::Line2_I;
				/*DropNode(Line2);*/
				break;
			case 'j':
			case 'J':
				NoteClick(GetNoteList(), Line3);
				CurInputNoteKey = InputNotePoint::Line3_I;
				//DropNode(Line3);
				break;
			case 'K':
			case 'k':
				NoteClick(GetNoteList(), Line4);
				CurInputNoteKey = InputNotePoint::Line4_I;
				//DropNode(Line4);
				break;
			case '+':
				volume += 0.1f;
				if (volume > 1.0f)
					volume = 1.0f;
				FMOD_Channel_SetVolume(channel, volume);
				break;
			case '-':
				volume -= 0.1f;
				if (volume < 0.0f)
					volume = 0.0f;
				FMOD_Channel_SetVolume(channel, volume);
				break;
			}
		}
	}
	SoundUpdate();
	NoteUpdate();
	MapUpdate();
}

void InGameRender()
{
	MapRender();
	NoteRender();
	ScoreRender();
	SoundTextRender();
}
void InGameRelease()
{
	SoundRelease();
	MapRelease();
	NoteRelease();
	ScreenRelease();

}
clock_t CurScore = 0;
clock_t OldScore = 0;
int ResultSelectNum = 0;
void ResultInit()
{
	OldScore = clock();
	ResultSelectNum = 0;
}

void ResultUpdate()
{
	int nKey;
	int KeyArrow = 0;
	if (_kbhit())
	{
		nKey = _getch();
		if (nKey == 'q')
		{
			isbSystemOn = false;
		}
		if (nKey == 224)
		{
			KeyArrow = _getch();
			switch (KeyArrow)
			{
			case 72:		//↑
				--ResultSelectNum;
				if (ResultSelectNum < 0)
					ResultSelectNum = 0;
				break;
			case 80:		//↓
				++ResultSelectNum;
				if (2 < ResultSelectNum)
					ResultSelectNum = ResultSelectNum - 1;
				break;
			}
		}
		switch (nKey)
		{
		case 13:		//엔터키
			if (ResultSelectNum == 0)			//메인메뉴
			{
				TitleInit();
				SceneState = Scene::TitleScene;
			}
			else if (ResultSelectNum == 1)
			{
				
				SceneState = Scene::NoteLoadScene;
			}
			break;
		}
	}
}
int ScoreIncreaseSpeed = 5;
int UpDateScore = 0;
void ResultRender()
{
	
	ScreenPrint(35, 0, "<결과>");
	char resultStr[100];
	CurScore = clock();
	if (UpDateScore < Score)
	{
		UpDateScore = (CurScore - OldScore) * ScoreIncreaseSpeed;
		sprintf(resultStr, "점수 : %d", (CurScore - OldScore) * ScoreIncreaseSpeed);
		ScreenPrint(35, 2, resultStr);
	}
	else
	{
		sprintf(resultStr, "점수 : %d", Score);
		ScreenPrint(35, 2, resultStr);
		UpDateScore = Score;
	}
	int RankSSS = NoteCount * 300;
	int RankSS = NoteCount * 200;
	int RankS = NoteCount * 100;
	if (UpDateScore <= RankSSS && RankSS < UpDateScore)		//SS랭크 최대점수와 SSS랭크 최대점수 사이
	{
		ScreenPrint(35, 4, "등급 : SSS");
	}
	else if (RankS < UpDateScore && UpDateScore <= RankSS)		//SS랭크 최대점수와 SSS랭크 최대점수 사이
	{
		ScreenPrint(35, 4, "등급 : SS");
	}
	else
	{
		ScreenPrint(35, 4, "등급 : S");
	}

	char str[256];
	sprintf(str, ">선택(↑ , ↓)< : %d", ResultSelectNum);
	ScreenPrint(35, 0, str);


	ScreenPrint(35, 8, "1.메뉴");
	ScreenPrint(35, 10, "2.다시하기");
	if (ResultSelectNum == 0)
	{
		SetColor(9);
		ScreenPrint(35, 8, "1.메뉴");
	}
	else if (ResultSelectNum == 1)
	{
		SetColor(9);
		ScreenPrint(35, 10, "2.다시하기");
	}
	SetColor(15);

}



void NoteInit()
{
	/*for (int i = 0; i < 10; i++)
	{
		NOTE note;

		note.point = (NotePoint)(i % 4);

		note.nX = 0;
		note.nY = 0;
		NoteList.push_back(note);
	}
*/
}
bool DeleteNote(NOTE DelNote)
{
	for (int i = 0; i < (int)NoteList.size(); i++)
	{
		if (NoteList[i].NoteKey == DelNote.NoteKey)
		{
			NoteList.erase(NoteList.begin() + i);
			return true;
		}
	}
	return false;
}
void DropNode(NotePoint point, int NoteKeyCount)
{
	NOTE note;

	note.point = point;

	note.nX = 0;
	note.nY = 0;
	note.CurTime = clock();
	note.OldTime = 0;
	note.NoteKey = NoteKeyCount;
	NoteList.push_back(note);
}

//해당라인에 노트가 존재한다면 (*스코어 할당 , 추가 이벤트 설정 구간)
int isNoteExistLinePoint(NOTE note)
{
	switch (note.point)
	{
	case Line1:
	case Line2:
	case Line3:
	case Line4:
		if (note.nY == MainDecision)
		{
			if (DeleteNote(note))
				result = FMOD_System_PlaySound(g_system, hitSound, 0, 0, &EffectSoundChannel);
			return 10;
		}
		else if (note.nY == SubDecision1 || note.nY == SubDecision1 - 1)
		{
			if (DeleteNote(note))
				result = FMOD_System_PlaySound(g_system, hitSound, 0, 0, &EffectSoundChannel);
			return 9;
		}
		else if (note.nY == SubDecision2 || note.nY == SubDecision2 + 1)
		{
			if (DeleteNote(note))
				result = FMOD_System_PlaySound(g_system, hitSound, 0, 0, &EffectSoundChannel);
			return 11;
		}
		break;
	}
	return -1;
}
void NoteClick(vector<NOTE> NoteList, NotePoint point)
{
	for (int i = 0; i < (int)NoteList.size(); i++)
	{
		if (NoteList[i].point == point)
		{
			//해당라인에 노트가 존재한다면 (*스코어 할당 , 추가 이벤트 설정 구간)
			if (isNoteExistLinePoint(NoteList[i]) == 10)			//중앙 라인 노트 존재여부 검출	
			{
				Score += 300;			//점수 부여
				ScoreToColor = 300;		//점수에 따른 색깔
				++Combo;;
			}
			else if (isNoteExistLinePoint(NoteList[i]) == 9)			//앞 라인 노트 존재여부 검출	
			{
				Score += 100;
				ScoreToColor = 100;
				++Combo;
			}
			else if (isNoteExistLinePoint(NoteList[i]) == 11)			//뒷 라인 노트 존재여부 검출	
			{
				Score += 200;
				ScoreToColor = 200;
				++Combo;
			}
		}
	}
}

int EndTime = 0;
void NoteUpdate()
{
	for (int i = 0; i < (int)NoteList.size(); i++)
	{
		switch (NoteList[i].point)		//라인에 따른 X좌표값 부여
		{
		case Line1:
			NoteList[i].nX = DefLine1;
			break;
		case Line2:
			NoteList[i].nX = DefLine2;
			break;
		case Line3:
			NoteList[i].nX = DefLine3;
			break;
		case Line4:
			NoteList[i].nX = DefLine4;
			break;

		default:
			break;
		}
		if (0 <= NoteList[i].nY && NoteList[i].nY <= MapYEndPoint)			//노트가 이동중일때
		{
			NoteList[i].nY += NoteSpeed;
		}

		if (NoteList[i].nY == MainDecision)			//노트가 판정선 중앙에 도착
		{
			NoteList[i].OldTime = NoteList[i].CurTime;
			NoteList[i].CurTime = clock();
			EndTime = NoteList[i].CurTime - NoteList[i].OldTime;			//노트가 생성된 시간 ~ 노트가 판정선에 닿았을 때의 시간
			if (isCustomMode)
				SaveNote(EndTime, NoteList[i].point);			//커스텀 모드일때 자동으로 생성된 노트들을 파일에 저장함
		}
		if (MapYEndPoint < NoteList[i].nY) {		//노트가 필드 밖으로 벗어났을때
			NoteList.erase(NoteList.begin() + (i));
			ScoreToColor = -1;
			Combo = 0;
		}
	}


}

void NoteRender()
{
	if (isCustomMode)
	{
		char str[100];
		sprintf(str, "생성된 노트수 : %d", NoteList.size());
		ScreenPrint(80, 2, str);

		char str2[100];
		sprintf(str2, "NoteEndTime: %d", EndTime);
		ScreenPrint(80, 15, str2);
	}
	for (int i = 0; i < (int)NoteList.size(); i++)
	{
		if (0 <= NoteList[i].nY && NoteList[i].nY <= MapYEndPoint)
		{
			ScreenPrint(NoteList[i].nX, NoteList[i].nY, NoteNode);

			/*for (int j = 0; j < 5; j++)
			{
				ScreenPrint(NoteList[i].nX, NoteList[i].nY + j, LongNoteNode[j]);
			}*/
		}
		if (isCustomMode)
		{
			char str2[256];
			sprintf(str2, "%d : %d", NoteList[i].nX, NoteList[i].nY);
			ScreenPrint(80, 22 + i, str2);
		}
	}
}
void NoteRelease()
{
}

void IntroduceUpdate()
{
	int nKey;

	nKey = _getch();

	if (nKey == 13)
	{
		TitleInit();
		SceneState = Scene::TitleScene;
	}
}

void IntroduceRender()
{
	ScreenPrint(0, 0, "조작키:");
	ScreenPrint(0, 1, "1.일반 모드");
	ScreenPrint(0, 2, "1번째라인 : D");
	ScreenPrint(0, 3, "2번째라인 : F");
	ScreenPrint(0, 4, "3번째라인 : J");
	ScreenPrint(0, 5, "4번째라인 : K");
	ScreenPrint(0, 6, "볼륨 조절 : + , -");
	ScreenPrint(0, 7, "2. 커스텀 자동 맵 제작 모드");
	ScreenPrint(0, 8, "커스텀 모드를 실행하면 자동으로 노트를 생성함");
	ScreenPrint(0, 9, "커스텀 모드를 통해서 노트 맵을 먼저 만들어야만");
	ScreenPrint(0, 10, "일반모드로 플래이 할 수 있음");
	ScreenPrint(0, 11, "되돌아가기 : Enter");
}

const int Line1_Left_PosX = 0;
const int Line1_Right_PosX = 16;

const int Line2_Left_PosX = 16;
const int Line2_Right_PosX = 32;

const int Line3_Left_PosX = 32;
const int Line3_Right_PosX = 48;

const int Line4_Left_PosX = 48;
const int Line4_Right_PosX = 64;

const int Print_D_KeyPos = (Line1_Left_PosX + Line1_Right_PosX) / 2;
const int Print_F_KeyPos = (Line2_Left_PosX + Line2_Right_PosX) / 2;
const int Print_J_KeyPos = (Line3_Left_PosX + Line3_Right_PosX) / 2;
const int Print_K_KeyPos = (Line4_Left_PosX + Line4_Right_PosX) / 2;
void MapInit()
{
	OldScoreToColor = clock();
}
int InputNoteKeyTimer = 500;
void MapUpdate()			//0.5초마다 눌렀던 라인을 표시함
{
	
	if (CurInputNoteKey != InputNotePoint::None)
	{
		clockCurInputNote = clock();
		if (clockCurInputNote - clockOldInputNote > InputNoteKeyTimer)
		{
			CurInputNoteKey = InputNotePoint::None;
			clockOldInputNote = clockCurInputNote;
		}
	}
}
int InputColorKey = 9;
void MapRender()				//맵 출력
{
	
	ScreenPrint(16, MapYPoint, "┬");
	ScreenPrint(16, MapYPoint + 1, "│");
	ScreenPrint(32, MapYPoint, "┬");
	ScreenPrint(32, MapYPoint + 1, "│");

	ScreenPrint(47, MapYPoint, "┬");
	ScreenPrint(47, MapYPoint + 1, "│");

	ScreenPrint(0, MapYPoint, "┌");
	ScreenPrint(0, MapYPoint + 1, "│");
	ScreenPrint(MapXPoint, MapYPoint, "┐");
	ScreenPrint(MapXPoint, (MapYPoint + 1), "│");


	
	//MainDecision = 45, SubDecision1 = 44 , SubDecision2 = 46
	if (CurInputNoteKey == InputNotePoint::Line1_I)
	{
		SetColor(InputColorKey);
	}
	ScreenPrint(0, MainDecision - 1, "│");
	ScreenPrint(0, MainDecision - 2, "│");
	ScreenPrint(0, MainDecision, "├");
	ScreenPrint(0, MainDecision + 1, "│");

	ScreenPrint(2, SubDecision1, SubDestLine);
	ScreenPrint(2, MainDecision, DestLine);
	ScreenPrint(2, SubDecision2, SubDestLine);
	ScreenPrint(Print_D_KeyPos, SubDecision2 + 4, "D");
	

	if (CurInputNoteKey == InputNotePoint::Line2_I)
	{
		SetColor(InputColorKey);
	}
	ScreenPrint(16, MainDecision - 1, "│");
	ScreenPrint(16, MainDecision - 2, "│");
	ScreenPrint(16, MainDecision, "┼");
	ScreenPrint(16, MainDecision + 1, "│");
	if (CurInputNoteKey == InputNotePoint::Line1_I)
	{
		SetColor(15);
	}
	

	ScreenPrint(18, SubDecision1, SubDestLine);
	ScreenPrint(18, MainDecision, DestLine);
	ScreenPrint(18, SubDecision2, SubDestLine);
	ScreenPrint(Print_F_KeyPos, SubDecision2 + 4, "F");
	

	if (CurInputNoteKey == InputNotePoint::Line3_I)
	{
		SetColor(InputColorKey);
	}
	ScreenPrint(32, MainDecision - 1, "│");
	ScreenPrint(32, MainDecision - 2, "│");
	ScreenPrint(32, MainDecision, "┼");
	ScreenPrint(32, MainDecision + 1, "│");
	if (CurInputNoteKey == InputNotePoint::Line2_I)
	{
		SetColor(15);
	}
	

	ScreenPrint(34, SubDecision1, SubDestLine);
	ScreenPrint(34, MainDecision, DestLine);
	ScreenPrint(34, SubDecision2, SubDestLine);
	ScreenPrint(Print_J_KeyPos, SubDecision2 + 4, "J");
	
	

	if (CurInputNoteKey == InputNotePoint::Line4_I)
	{
		SetColor(InputColorKey);
	}
	ScreenPrint(48, MainDecision - 1, "│");
	ScreenPrint(48, MainDecision - 2, "│");
	ScreenPrint(48, MainDecision, "┼");
	ScreenPrint(48, MainDecision + 1, "│");
	if (CurInputNoteKey == InputNotePoint::Line3_I)
	{
		SetColor(15);
	}

	ScreenPrint(50, SubDecision1, SubDestLine);
	ScreenPrint(50, MainDecision, DestLine);
	ScreenPrint(50, SubDecision2, SubDestLine);
	ScreenPrint(Print_K_KeyPos, SubDecision2 + 4, "K");


	ScreenPrint(64, MainDecision - 1, "│");
	ScreenPrint(64, MainDecision - 2, "│");
	ScreenPrint(64, MainDecision, "┤");
	ScreenPrint(64, MainDecision + 1, "│");
	if (CurInputNoteKey == InputNotePoint::Line4_I)
	{
		SetColor(15);
	}
	



	if (CurInputNoteKey != InputNotePoint::None)
	{
		for (int i = 3; i <= 10; i++)
		{
			switch (CurInputNoteKey)
			{
			case Line1_I:
				SetColor(InputColorKey);
				ScreenPrint(Line1_Left_PosX, MainDecision - i, "│");
				ScreenPrint(Line1_Right_PosX, MainDecision - i, "│");
				SetColor(15);
				break;
			case Line2_I:
				SetColor(InputColorKey);
				ScreenPrint(Line2_Left_PosX, MainDecision - i, "│");
				ScreenPrint(Line2_Right_PosX, MainDecision - i, "│");
				SetColor(15);
				break;
			case Line3_I:
				SetColor(InputColorKey);
				ScreenPrint(Line3_Left_PosX, MainDecision - i, "│");
				ScreenPrint(Line3_Right_PosX, MainDecision - i, "│");
				SetColor(15);
				break;
			case Line4_I:
				SetColor(InputColorKey);
				ScreenPrint(Line4_Left_PosX, MainDecision - i, "│");
				ScreenPrint(Line4_Right_PosX, MainDecision - i, "│");
				SetColor(15);
				break;
			}
			
		}
	}
}
void MapRelease()
{

}
void ScoreRender()
{
	int PrintPosX = 40;
	int PrintPosY = 10;
	if (ScoreToColor != 0)
	{
		switch (ScoreToColor)
		{
		case 100:
			SetColor(3);
			ScreenPrint(PrintPosX, PrintPosY, "<< 100 >>");
			break;
		case 200:
			SetColor(10);
			ScreenPrint(PrintPosX, PrintPosY, "<< 200 >>");
			break;
		case 300:
			SetColor(14);
			ScreenPrint(PrintPosX, PrintPosY, "<< 300 >>");
			break;
		case -1:
			SetColor(8);
			ScreenPrint(PrintPosX, PrintPosY, "<< Miss >>");
			break;
		}
		SetColor(15);
		
		char comboStr[10];
		sprintf(comboStr, "%d", Combo);
		SetColor(8);
		ScreenPrint(PrintPosX + 5, PrintPosY + 1, comboStr);
		SetColor(15);
	}
}
FMOD_DSP_PARAMETER_FFT* GetFFTPARAMETER(FMOD_SOUND *sound)
{
	result = FMOD_System_PlaySound(g_system, sound, 0, 0, &channel);
	result = FMOD_Channel_AddDSP(channel, FMOD_CHANNELCONTROL_DSP_HEAD, dsp);
	result = FMOD_DSP_GetParameterData(dsp, FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fft, 0, 0, 0);
	return fft;
}
const char * SoundFileName = "SoundList.txt";
//FMOD Func
void SoundInit()
{
	printf("로딩중입니다...");
	result = FMOD_System_Create(&g_system);
	result = FMOD_System_Init(g_system, 32, FMOD_INIT_NORMAL, NULL);
	SoundCount = sizeof(SoundList) / sizeof(const char *);
	for (int i = 0; i < SoundCount; i++)
	{
		FMOD_SOUND *snd;
		sound.push_back(snd);

		char str[256] = "\0";
		sprintf(str, "%s.mp3", SoundList[i]);

		//FMOD_System_CreateSound(g_system, str, FMOD_DEFAULT, 0, &sound[i]);
		FMOD_System_CreateStream(g_system, str, FMOD_DEFAULT, 0, &sound[i]);
	}
	result = FMOD_System_CreateDSPByType(g_system, FMOD_DSP_TYPE_FFT, &dsp);
	FMOD_System_CreateSound(g_system, "soft-hitclap.wav", FMOD_DEFAULT, 0, &hitSound);
	FMOD_Channel_SetVolume(channel, EffectSoundVolume);

	
	
	
}
void CheckNote()
{
	CurSearchTime = clock();
	if (CurSearchTime - OldSearchTime > GenerationCycle)
	{
		Balance = 0;
		for (int lenght = 0; lenght < fft->length; lenght++) {
			freqVal = fft->spectrum[0][lenght];
			if (freqVal >= VisualizerFilter)
			{
				Balance++;
			}
		}
		UpLoadNode(Balance);

		OldSearchTime = CurSearchTime;
	}
}



void SaveNote(int NoteDestTime, NotePoint point)
{
	char SaveFileName[200];
	sprintf(SaveFileName, "%s.txt", SoundList[SoundKeyNum]);
	fp = fopen(SaveFileName, "a");
	FMOD_Channel_GetPosition(channel, &CurPlaySoundProcesstime, FMOD_TIMEUNIT_MS);		//현재 곡 진행시간
	int PointLine = (int)point;
	
	fprintf(fp, "%d %d %d %d %f\n", CurPlaySoundProcesstime, NoteDestTime, PointLine, NoteSpeed, pitch);

	fclose(fp);
}

bool LoadNote(const char * LoadMusicFileName)
{
	char NoteProcess[100];
	int temp = 0;
	char file[256];
	char * ptr;
	
	sprintf(file, "%s.txt", LoadMusicFileName);
	fp = fopen(file, "rt");
	if (fp == NULL)			//파일이 존재하지 않으면 종료
	{
		isbSystemOn = false;
		return false;
	}
	else if (fp != NULL)
	{
		while (!feof(fp))
		{
			LOADNOTE noteLoad;
			int TimeProcess;
			fgets(NoteProcess, 100, fp);

			ptr = strtok(NoteProcess, " ");

			for (int cnt = 0; ptr != NULL; cnt++)
			{
				switch (cnt)
				{
				case 0:
					TimeProcess = atoi(ptr);
					noteLoad.NoteTime = TimeProcess;
					break;
				case 1:
					noteLoad.NoteSyncTime = atoi(ptr);
					break;
				case 2:
					noteLoad.NoteLine = atoi(ptr);
					break;
				case 3:
					noteLoad.Speed = atoi(ptr);
					break;
				case 4:
					noteLoad.Pitch = (float)atof(ptr);
					break;
				default:
					break;
				}
				ptr = strtok(NULL, " ");
			}


			//process -= NoteSync;
			NoteTimeData.push_back(noteLoad);
			//return false;
		}
	}
	fclose(fp);
	return true;
}

void UpLoadNode(NotePoint point)
{
	++NoteCount;
	int NodeLine;
	if (isCustomMode)
	{
		NodeLine = rand() % 4;
	}
	else
	{
		NodeLine = (int)point;
	}

	switch (NodeLine)
	{
	case 0:
		DropNode(Line1, NoteCount);
		break;
	case 1:
		DropNode(Line2, NoteCount);
		break;
	case 2:
		DropNode(Line3, NoteCount);
		break;
	case 3:
		DropNode(Line4, NoteCount);
		break;
	}

}
void UpLoadNode(int Balance)
{
	if (Balance > OutPutNodeFilter)
	{
		++NoteCount;
		int NodeLine = rand() % 4;
		switch (NodeLine)
		{
		case 0:
			DropNode(Line1, NoteCount);
			break;
		case 1:
			DropNode(Line2, NoteCount);
			break;
		case 2:
			DropNode(Line3, NoteCount);
			break;
		case 3:
			DropNode(Line4, NoteCount);
			break;
		}
	}
}
int RecordNoteCount = 0;
int NextNoteTime = 0;
void SoundUpdate()
{
	//GetFFTPARAMETER(sound[SoundKeyNum]);
	clockAccumulate = clock();
	if (isCustomMode)
		CheckNote();
	else
	{
		FMOD_Channel_GetPosition(channel, &CurPlaySoundProcesstime, FMOD_TIMEUNIT_MS);		//현재 곡 진행시간

		if (RecordNoteCount < (int)NoteTimeData.size())
		{
			NextNoteTime = NoteTimeData[RecordNoteCount].NoteTime;
			NextNoteTime -= NoteTimeData[RecordNoteCount].NoteSyncTime;		//노트 싱크를 맞추기 위해서 뺴준다.
			if (NextNoteTime - 50 <= (int)CurPlaySoundProcesstime && (int)CurPlaySoundProcesstime <= NextNoteTime + 50)
			{
				NoteSpeed = NoteTimeData[RecordNoteCount].Speed;
				if (!NoteTimeData.empty())
				{
					if (NoteTimeData[RecordNoteCount].Pitch != pitch)
					{
						pitch = NoteTimeData[RecordNoteCount].Pitch;
						FMOD_Channel_SetPitch(channel, pitch);
					}
				}
				UpLoadNode((NotePoint)NoteTimeData[RecordNoteCount].NoteLine);
				++RecordNoteCount;

			}
		}
		
	}
	FMOD_Sound_GetLength(sound[SoundKeyNum], &SoundLength, FMOD_TIMEUNIT_MS);
	//SoundLength 노래의 최대 시간 , CurPlaySoundProcesstime 노래의 진행시간
	if (CurPlaySoundProcesstime > SoundLength - 100 && isCustomMode == false)
	{
		SceneState = Scene::ResultScene;
		ResultInit();
	}
	else if (CurPlaySoundProcesstime > SoundLength - 100 && isCustomMode == true)
	{
		SelectInit();
		SceneState = Scene::SelectScene;
	}
	FMOD_System_Update(g_system);
}
void SoundTextRender()
{
	if (isCustomMode)
	{
		ScreenPrint(80, 2, "<맵 제작 모드>");
		ScreenPrint(80, 3, "자동으로 노트생성중..");


		char str[40];
		sprintf(str, "(p,l)NoteUnit : %f", VisualizerFilter);			//노트 생성 필터
		ScreenPrint(80, 4, str);

		char str1[40];
		sprintf(str1, "NoteTime : %d", CurSearchTime - OldSearchTime);
		ScreenPrint(80, 6, str1);

		char str2[40];
		sprintf(str2, "OutPutNodeFilter : %d", OutPutNodeFilter);			//노트 생성 필터
		ScreenPrint(80, 8, str2);

		char str3[40];
		sprintf(str3, "(+,-)현재볼륨 : %f", volume);			//볼륨
		ScreenPrint(80, 10, str3);

		char str4[40];
		sprintf(str4, "pitch : %f", pitch);			//노트 음정
		ScreenPrint(80, 12, str4);

		char str5[40];
		sprintf(str5, "(],[)NoteSpeed : %d", NoteSpeed);			//노트가 이동하는 스피드
		ScreenPrint(80, 14, str5);

		char str6[40];
		sprintf(str6, "NoteCount : %d", NoteCount);			//지금까지 출력된 누적 노트 개수
		ScreenPrint(80, 16, str6);

		char str7[40];
		sprintf(str7, "GameScore : %d", Score);			//게임 스코어
		ScreenPrint(80, 18, str7);

		char str8[40];
		sprintf(str8, "(m,n)GenerateCycle : %d", GenerationCycle);			//노트 생성 주기
		ScreenPrint(80, 20, str8);

		char str9[40];
		sprintf(str9, "AccTimer : %d", CurPlaySoundProcesstime);			//누적 시간
		ScreenPrint(80, 19, str9);

		//char str10[40];
		//sprintf(str10, "RecordNote : %d", NextNoteTime);			//누적 시간
		//ScreenPrint(80, 35, str10);
	}
	else
	{
		ScreenPrint(80, 16, "<일반 플레이>");

		char str7[40];
		sprintf(str7, "GameScore : %d", Score);			//게임 스코어
		ScreenPrint(80, 18, str7);

		char str3[40];
		sprintf(str3, "현재볼륨 : %f", volume);			//볼륨
		ScreenPrint(80, 20, str3);

		
	}
	FMOD_Channel_GetPosition(channel, &CurPlaySoundProcesstime, FMOD_TIMEUNIT_MS);		//현재 곡 진행시간

	FMOD_Sound_GetLength(sound[SoundKeyNum], &SoundLength, FMOD_TIMEUNIT_MS);

	GetTransformTimeString(CurPlaySoundProcesstime, &curTimeSec, &curTimeMin);
	GetTransformTimeString(SoundLength, &MaxTimeSec, &MaxTimeMin);
	char MusicTime[100];
	sprintf(MusicTime, "%d:%d : %d:%d", curTimeMin, curTimeSec, MaxTimeMin, MaxTimeSec);

	char str9[40];
	sprintf(str9, "시간:%s", MusicTime);			//누적 시간
	ScreenPrint(80, 22, str9);
}

void SoundRelease()
{
	FMOD_Sound_Release(hitSound);
	for (int i = 0; i < SoundCount; i++)
		FMOD_Sound_Release(sound[i]);

	FMOD_System_Close(g_system);

	FMOD_System_Release(g_system);
}


void GetTransformTimeString(unsigned int time, int * Sec, int * Min)
{
	int TimeSecond = (int)(time / 1000);
	int minute = TimeSecond / 60;
	int Second = TimeSecond % 60;

	*Sec = Second;
	*Min = minute;


}

int GetnTransformTime(unsigned int time)
{
	return (int)(time / 1000);
}