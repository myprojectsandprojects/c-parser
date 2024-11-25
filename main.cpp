
/*
preprocessor directives, comments
values (string, char, number(int, float))
identifier (keyword, variable, type, macro, enum value, label, ...?)
operators:
	arithmetic,
	comparison (==, !=, <, >, <=, >=),
	logical (&&, ||, !),
	bitwise (&, |, ~, ^, <<, >>),
	assignment ( '=', and combinations with other operators),
	member and pointer ([], *, &, ., ->)
	(';', ',', '?', ':', sizeof, alignof?, (, ), {, },  ...)
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <assert.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

enum tokenType {
	token_BLOCKCOMMENT = 0,
	token_LINECOMMENT,
	token_IDENTIFIER,
	token_STRING,
	token_CHAR,
	token_NUMBER,
	token_PREPROCESSOR,
	token_KEYWORD_STRUCT,
	token_KEYWORD_ENUM,
	token_KEYWORD_UNION,
	token_KEYWORD_CONST,
	token_KEYWORD_FLOAT,
	token_KEYWORD_DOUBLE,
	token_KEYWORD_TYPEDEF,
	token_KEYWORD_INT,
	token_KEYWORD_CHAR,
	token_KEYWORD_SHORT,
	token_KEYWORD_LONG,
	token_KEYWORD_SIGNED,
	token_KEYWORD_UNSIGNED,
	token_KEYWORD_VOID,
	token_KEYWORD_STATIC,
	token_UNKNOWN,

	/* 32 - 127 reserved for tokens whose type is their ascii value */
};
const char *TokenNames[] = {
	"block-comment",
	"line-comment",
	"identifier",
	"string",
	"char",
	"number",
	"preprocessor",
	"keyword-struct",
	"keyword-enum",
	"keyword-union",
	"keyword-const",
	"keyword-float",
	"keyword-double",
	"keyword-typedef",
	"keyword-int",
	"keyword-char",
	"keyword-short",
	"keyword-long",
	"keyword-signed",
	"keyword-unsigned",
	"keyword-void",
	"keyword-static",
	"unknown",
};

struct token {
	tokenType Type;
	int Start, End;
};

//#define MAX_TOKENS 1024
struct tokens {
	int Count;
	int MaxCount;
//	token Data[MAX_TOKENS];
	token *Data;
};

//void tokens_init(tokens *Tokens) {
tokens tokens_new(token *Memory, int MemoryTokensCount) {
	tokens Tokens;
	Tokens.Count = 0;
	Tokens.Data = Memory;
	Tokens.MaxCount = MemoryTokensCount;
	return Tokens;
}

void tokens_add(tokens *Tokens, token Token) {
	assert(Tokens->Count < Tokens->MaxCount);
	Tokens->Data[Tokens->Count] = Token;
	Tokens->Count += 1;
}

////const int MAX_TOKENS = 1024;
//const int MAX_TOKENS = 16 * 1024;
//token Tokens[MAX_TOKENS];
//int TokensCount = 0;

#define TokensMemoryMax 1024
token TokensMemory[TokensMemoryMax];

//int TokenIndex = 0;
//while(true) {
//	// int, unsigned short int, const char *,
//	// [static] type_ident func_ident ([type ident [, type ident]]) { [statements] }
//
//	// any # of keywords, IDENT, optional *, IDENT, '(', any number of arguments, ')', '{', any number of statements, '}'
//	if(Tokens[TokenIndex].type == token_IDENTIFIER)
//}

/* we expect a character literal */
			/*
			'a', '\[escape sequence character]', '\[3 octal digits]', '\x[2 hexadecimal digits]'
			C character literal is 4 bytes wide, which means that 'ab', 'abc', 'abcd' are valid (though useless)
			'%%' -- printf format strings only
			*/

//void ValidateCharLiteral() {
//bool IsError = false;
//			if(Contents[i] == '\0'){
//				// error: unexpected '\0'
//				IsError = true;
////				EndToken = i;
//			}else if(Contents[i] == '\''){
//				// error: empty character literal
//				IsError = true;
////				EndToken = i;
//			}else if(Contents[i] == '\n'){
//				// error: character literal can't contain a '\n'
//				IsError = true;
////				EndToken = i;
//			}else if(Contents[i] == '\\'){
//				// possible escape sequence
//				i += 1;
//				if(Contents[i] == '\\' || Contents[i] == '\'' || Contents[i] == 'n'  || Contents[i] == 'r' || Contents[i] == 'b' || Contents[i] == 't' || Contents[i] == 'f' || Contents[i] == 'a' || Contents[i] == '?'){
//					// ok
//					i += 1;
//					if(Contents[i] == '\''){
//						// ok
////						i += 1;
//					}else{
//						// error
//						//@ C actually supports up to 4 characters in a literal
//						IsError = true;
//					}
//				}else if(Contents[i] == 'x'){
//					// hex number: expecting 2 hex digits
//					bool IsHex = true;
//					i += 1;
//					if(!((Contents[i] >= '0' && Contents[i] <= '9') || (Contents[i] >= 'A' && Contents[i] <= 'F'))){
//						IsHex = false;
//					}else{
//						i += 1;
//						if(!((Contents[i] >= '0' && Contents[i] <= '9') || (Contents[i] >= 'A' && Contents[i] <= 'F'))){
//							IsHex = false;
//						}
//					}
//					
//				}else if(false){
//					// octal number: expecting 3 ocatal digits
//				}else{
//					// error: unknown escape sequence character
//					IsError = true;
//				}
//				//@ octals and hexadecimals
////				EndToken = i;
//			}else{
//				// any character
//				i += 1;
//				if(Contents[i] == '\''){
//					// ok
////					i += 1;
//				}else{
//					// error
//					//@ C actually supports up to 4 characters in a literal
//					IsError = true;
////					EndToken = i;
//				}
////				EndToken = i;
//			}
//}

bool IsTypeToken(tokenType Type) {
	return (Type == token_KEYWORD_CONST
		|| Type == token_KEYWORD_FLOAT
		|| Type == token_KEYWORD_DOUBLE
		|| Type == token_KEYWORD_INT
		|| Type == token_KEYWORD_CHAR
		|| Type == token_KEYWORD_SHORT
		|| Type == token_KEYWORD_LONG
		|| Type == token_KEYWORD_SIGNED
		|| Type == token_KEYWORD_UNSIGNED
		|| Type == token_KEYWORD_VOID);
}

char *RangeToString(char *TempBuffer, const char *Contents, int Start, int End) {
	int RangeLength = End - Start;
//	printf("range length: %d\n", RangeLength);
	strncpy(TempBuffer, &Contents[Start], RangeLength);//@overflow check
	TempBuffer[RangeLength] = '\0';

	return TempBuffer;
}

////@bounds check
//void PrintRange(char Buffer[], int Start, int End) {
//	char PrintBuffer[1024];
//
////	int RangeLength = End - Start;
////	strncpy(PrintBuffer, &Buffer[Start], RangeLength);//@overflow check
////	PrintBuffer[RangeLength] = '\0';
//	RangeToString(PrintBuffer, Buffer, Start, End);
//
//	printf("%s", PrintBuffer);
//}

void PrintRange(char *Contents, int Start, int End) {
	char Ch = Contents[End];
	Contents[End] = '\0';
	printf("%s", &Contents[Start]);
	Contents[End] = Ch;
}

//@ is this correct?
bool IsRangeEqual(const char *Buffer, int Start, int End, const char *Str) {
	int Index = 0;
	while(Str[Index] != '\0' && ((Start + Index) < End)) {
		if(Str[Index] != Buffer[Start + Index]) {
			break;
		}
		Index += 1;
	}
	return ((Str[Index] == '\0') && ((Start + Index) == End));
}

int main(){
	const char *FilePath = "new-test-parsing.c";
//	const char *FilePath = "test/main.c";
//	const char *FilePath = "test-parsing.c";
//	const char *FilePath = "test-language/test.c";
//	const char *FilePath = "test-lexing.c";
//	const char *FilePath = "main.cpp";
//	const char *FilePath = "/home/eero/C-parser/main-copy.cpp";

	int File = open(FilePath, O_RDONLY);
	if(File == -1){
		fprintf(stderr, "error: open(): %s\n", strerror(errno));
		return 1;
	}
	printf("successfully opened the file\n");

	struct stat FileInfo;
	if(fstat(File, &FileInfo) == -1){
		fprintf(stderr, "error: open(): %s\n", strerror(errno));
		return 1;
	}
	off_t FileSize = FileInfo.st_size;
	printf("file size: %ld\n", FileSize);

	// docs:
	// we get a multiple of page size
	//long PageSize = sysconf(_SC_PAGE_SIZE);
	// remaining bytes will be 0'd
	//@ if underlying file on disk changes?
//	char *Contents = (char *)mmap(NULL, FileSize + 1, PROT_READ, MAP_PRIVATE, File, 0);
	char *Contents = (char *)mmap(NULL, FileSize + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE, File, 0);
	if(Contents == MAP_FAILED){
		fprintf(stderr, "error: mapping the file\n");
		return 1;
	}
	printf("successfully mapped the file\n");

	if(close(File) == -1){
		fprintf(stderr, "error: closing the file\n");
		return 1;
	}
	printf("successfully closed the file\n");

	char TempBuffer[1024];
//	char TempBuffer[11852+1];

	tokens Tokens2 = tokens_new(TokensMemory, TokensMemoryMax);
	/*
	/ / -> '\n'
	/ * -> * /
	" -> "
	' -> '
	_ [abc] -> as long as _ [abc] [123]
	[digit] -> as long as [abc] [123]
	# -> '\n'
	*/

	int i = 0;
	while(true){
//	while(i < FileSize){
		while(Contents[i] == ' ' || Contents[i] == '\t' || Contents[i] == '\n'){
			i += 1;
		}
		if(Contents[i] == '\0'){
			break;
		}else if(Contents[i] == '/' && Contents[i+1] == '/'){
			int StartComment = i;
			i += 2; //@ '\0'?
			while(Contents[i] != '\0'){
				if(Contents[i] == '\n'){
					if(Contents[i-1] != '\\'){
						break;
					}
				}
				i += 1;
			}
			int EndComment = i;

//			Tokens[TokensCount].Type = token_LINECOMMENT;
//			Tokens[TokensCount].Start = StartComment;
//			Tokens[TokensCount].End = EndComment;
//			TokensCount += 1;
		} else if(Contents[i] == '/' && Contents[i+1] == '*'){
			int StartComment = i;
			i += 2;
			while(Contents[i] != '\0'){
				if(Contents[i] == '*' && Contents[i+1] == '/'){
					break;
				}
				i += 1;
			}
			if(Contents[i] != '\0') {
				i += 2;
			}
			int EndComment = i;

//			Tokens[TokensCount].Type = token_BLOCKCOMMENT;
//			Tokens[TokensCount].Start = StartComment;
//			Tokens[TokensCount].End = EndComment;
//			TokensCount += 1;
		} else if(Contents[i] == '"'){
//			i += 1;
			int StartToken = i;

			do{
				i += 1;
				if(Contents[i] == '"'){
					int n = 0;
					while(Contents[i - 1 - n] == '\\') {
						n += 1;
					}
					if(!(n & 1)) {
						break;
					}
				}
			}while(Contents[i] != '\0');

			if(Contents[i] != '\0'){
				i += 1;
			}

			int EndToken = i;

//			Tokens[TokensCount].Type = token_STRING;
//			Tokens[TokensCount].Start = StartToken;
//			Tokens[TokensCount].End = EndToken;
//			TokensCount += 1;
			tokens_add(&Tokens2, {token_STRING, StartToken, EndToken});
		} else if(Contents[i] == '\''){
//			i += 1;
			int StartToken = i;

			do{
				i += 1;
				if(Contents[i] == '\''){
					int n = 0;
					while(Contents[i - 1 - n] == '\\') {
						n += 1;
					}
					if(!(n & 1)) {
						break;
					}
				}
			}while(Contents[i] != '\0');

			if(Contents[i] != '\0'){
				i += 1;
			}

			int EndToken = i;

//			Tokens[TokensCount].Type = token_CHAR;
//			Tokens[TokensCount].Start = StartToken;
//			Tokens[TokensCount].End = EndToken;
//			TokensCount += 1;
			tokens_add(&Tokens2, {token_CHAR, StartToken, EndToken});

//			if(IsError){
//				printf("error: not a valid char literal: %s\n", TempBuffer);
//			}else{
//				printf("character literal: %s\n", TempBuffer);
//			}
		}else if(Contents[i] == '_' || isalpha(Contents[i])){
			int Start = i;
			do{
				i += 1;
			}while(isalnum(Contents[i]) || Contents[i] == '_');
			int End = i;

			tokenType Type;
			if(IsRangeEqual(Contents, Start, End, "struct")) {
				Type = token_KEYWORD_STRUCT;
			} else if(IsRangeEqual(Contents, Start, End, "enum")) {
				Type = token_KEYWORD_ENUM;
			} else if(IsRangeEqual(Contents, Start, End, "union")) {
				Type = token_KEYWORD_UNION;
			} else if(IsRangeEqual(Contents, Start, End, "const")) {
				Type = token_KEYWORD_CONST;
			} else if(IsRangeEqual(Contents, Start, End, "float")) {
				Type = token_KEYWORD_FLOAT;
			} else if(IsRangeEqual(Contents, Start, End, "double")) {
				Type = token_KEYWORD_DOUBLE;
			} else if(IsRangeEqual(Contents, Start, End, "typedef")) {
				Type = token_KEYWORD_TYPEDEF;
			} else if(IsRangeEqual(Contents, Start, End, "int")) {
				Type = token_KEYWORD_INT;
			} else if(IsRangeEqual(Contents, Start, End, "char")) {
				Type = token_KEYWORD_CHAR;
			} else if(IsRangeEqual(Contents, Start, End, "short")) {
				Type = token_KEYWORD_SHORT;
			} else if(IsRangeEqual(Contents, Start, End, "long")) {
				Type = token_KEYWORD_LONG;
			} else if(IsRangeEqual(Contents, Start, End, "signed")) {
				Type = token_KEYWORD_SIGNED;
			} else if(IsRangeEqual(Contents, Start, End, "unsigned")) {
				Type = token_KEYWORD_UNSIGNED;
			} else if(IsRangeEqual(Contents, Start, End, "void")) {
				Type = token_KEYWORD_VOID;
			} else if(IsRangeEqual(Contents, Start, End, "static")) {
				Type = token_KEYWORD_STATIC;
			} else {
				Type = token_IDENTIFIER;
			}
//			Tokens[TokensCount].Start = Start;
//			Tokens[TokensCount].End = End;
//			TokensCount += 1;
			tokens_add(&Tokens2, {Type, Start, End});
		}else if(isdigit(Contents[i])){
			int StartToken = i;
			do{
				i += 1;
				if(!isalnum(Contents[i]) && Contents[i] != '.'){
					break;
				}
			}while(Contents[i] != '\0');
			int EndToken = i;

//			Tokens[TokensCount].Type = token_NUMBER;
//			Tokens[TokensCount].Start = StartToken;
//			Tokens[TokensCount].End = EndToken;
//			TokensCount += 1;
			tokens_add(&Tokens2, {token_NUMBER, StartToken, EndToken});
		}else if(Contents[i] == '#'){
			// preprocessor directive
			int StartToken = i;
			i += 1;
			int Backslashes = 0;
			while(Contents[i] != '\0'){
				if(Contents[i] == '\n'){
					if(Backslashes != 1){
						break;
					}
					Backslashes = 0;
				}else if(Contents[i] == '\\'){
					Backslashes += 1;
				}else{
					Backslashes = 0;
				}
				i += 1;
			}
			int EndToken = i;

//			Tokens[TokensCount].Type = token_PREPROCESSOR;
//			Tokens[TokensCount].Start = StartToken;
//			Tokens[TokensCount].End = EndToken;
//			TokensCount += 1;
			tokens_add(&Tokens2, {token_PREPROCESSOR, StartToken, EndToken});
		}else if(Contents[i] == '(' || Contents[i] == ')' || Contents[i] == '*' || Contents[i] == '{' || Contents[i] == '}' || Contents[i] == ';' || Contents[i] == ',' || Contents[i] == '=' || Contents[i] == '[' || Contents[i] == ']'){
			int StartToken = i;
			i += 1;
			int EndToken = i;

//			Tokens[TokensCount].Type = (tokenType)Contents[i - 1];
//			Tokens[TokensCount].Start = StartToken;
//			Tokens[TokensCount].End = EndToken;
//			TokensCount += 1;
			tokens_add(&Tokens2, {(tokenType)Contents[i - 1], StartToken, EndToken});
		}else{
			int StartToken = i;
			i += 1;
			int EndToken = i;

//			Tokens[TokensCount].Type = token_UNKNOWN;
//			Tokens[TokensCount].Start = StartToken;
//			Tokens[TokensCount].End = EndToken;
//			TokensCount += 1;
			tokens_add(&Tokens2, {token_UNKNOWN, StartToken, EndToken});
		}
	}

//	printf("\n*** tokens: ***\n");
//	for(int TokenIndex = 0; TokenIndex < TokensCount; ++TokenIndex) {
//		token *Token = &Tokens[TokenIndex];
//
//		int TokenLength = Token->End - Token->Start;
//		strncpy(TempBuffer, &Contents[Token->Start], TokenLength);//@bounds
//		TempBuffer[TokenLength] = '\0';
//
//		if(Token->Type >= 32 && Token->Type <= 127) {
//			printf("'%c': %s\n", Token->Type, TempBuffer);
//		} else {
//			const char *TokenName = TokenNames[Token->Type];
//			printf("%s: %s\n", TokenName, TempBuffer);
//		}
//	}
//	puts("");
//	puts("");

//	printf("\n*** tokens2: ***\n");
//	for(int TokenIndex = 0; TokenIndex < Tokens2.Count; ++TokenIndex) {
//		token *Token = &Tokens2.Data[TokenIndex];
//
//		if(Token->Type >= 32 && Token->Type <= 127) {
//			printf("'%c': %s\n", Token->Type, RangeToString(TempBuffer, Contents, Token->Start, Token->End));
//		} else {
//			const char *TokenName = TokenNames[Token->Type];
//			printf("%s: %s\n", TokenName, RangeToString(TempBuffer, Contents, Token->Start, Token->End));
//		}
//	}
//	puts("");
//	puts("");


	/* parsing */
	/*
	goal is to find global names for types (tagged and non-tagged), variables and functions (declarations and definitions)
	if the code contains errors, pretty much undefined stuff happens
	otherwise it should work
	C99?
	*/
	int TokensCount = Tokens2.Count;
	token *Tokens = Tokens2.Data;
	puts("");
	puts("");
	for(int TokenIndex = 0; TokenIndex < TokensCount;) {
		if(Tokens[TokenIndex].Type == token_KEYWORD_TYPEDEF) {
			int Index = TokenIndex;

			Index += 1; assert(Index < TokensCount);

			while(IsTypeToken(Tokens[Index].Type) || Tokens[Index].Type == token_IDENTIFIER) {
				Index += 1; assert(TokenIndex < TokensCount);
			}

			if(
			Tokens[Index].Type == token_KEYWORD_STRUCT
			|| Tokens[Index].Type == token_KEYWORD_ENUM
			|| Tokens[Index].Type == token_KEYWORD_UNION
			) {
				Index += 1; assert(TokenIndex < TokensCount);
				if(Tokens[Index].Type == token_IDENTIFIER) {
					printf("struct, enum or union name: %s\n", RangeToString(TempBuffer, Contents, Tokens[Index].Start, Tokens[Index].End));
					Index += 1; assert(TokenIndex < TokensCount);
				} else {
					printf("anonymous struct, enum or union\n");
					assert(Tokens[Index].Type == '{');
				}
				
				if(Tokens[Index].Type == '{') {
					int ScopeCount = 0;
					do {
						Index += 1; if(Index == TokensCount) {break;}
	
						if(Tokens[Index].Type == '{') {
							ScopeCount += 1;
						}
						if(Tokens[Index].Type == '}') {
							if(ScopeCount == 0) {
								break;
							} else {
								ScopeCount -= 1;
							}
						}
					} while(Index < TokensCount);
					assert(Index < TokensCount);
					Index += 1; assert(TokenIndex < TokensCount);
				}

				while(Tokens[Index].Type == token_KEYWORD_CONST) {
					Index += 1; assert(TokenIndex < TokensCount);
				}
			}

			/*
			our possible position: X
			typestuff newtypeX
			typestuff X*newtype
			typestuff newtypeX[]
			typestuff X*newtype[]
			we want to be either at the first '*' or at the new name if no '*'
			typestuff X(*newtype[])(args)
			if func ptr, we want to be at '('
			*/
			if(Tokens[Index].Type == ';' || Tokens[Index].Type == ',' || Tokens[Index].Type == '[') {
				Index -= 1;
			} else {
//				printf("range debug: %s\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));
				assert(Tokens[Index].Type == '(' || Tokens[Index].Type == '*' || Tokens[Index].Type == token_IDENTIFIER);
			}

			// typedef oldtype newtype1, newtype2, newtype3, ...;
			while(true) {
				while(Tokens[Index].Type == '*' || Tokens[Index].Type == token_KEYWORD_CONST) {
					Index += 1; assert(Index < TokensCount);
				}

				if(Tokens[Index].Type == '(') {
					// func pointer or array pointer
					Index += 1; assert(Index < TokensCount);

					// ?
					while(Tokens[Index].Type == '*' || Tokens[Index].Type == token_KEYWORD_CONST) {
						Index += 1; assert(Index < TokensCount);
					}

					assert(Tokens[Index].Type == token_IDENTIFIER);
					printf("new function pointer type name: %s\n", RangeToString(TempBuffer, Contents, Tokens[Index].Start, Tokens[Index].End));
					Index += 1; assert(Index < TokensCount);

					while(Tokens[Index].Type == '[') {
						do {Index += 1; assert(Index < TokensCount);} while(Tokens[Index].Type != ']'); //@ end of tokens!
						Index += 1; assert(Index < TokensCount);
					}
					assert(Tokens[Index].Type == ')');

					Index += 1; assert(Index < TokensCount);
//					assert(Tokens[Index].Type == '(');
//					do {Index += 1; assert(TokenIndex < TokensCount);} while(Tokens[Index].Type != ')');
//					Index += 1; assert(TokenIndex < TokensCount);

					if(Tokens[Index].Type == '(') {
						// pointer to a function
						do {Index += 1; assert(Index < TokensCount);} while(Tokens[Index].Type != ')');
					} else {
						// pointer to a array
						assert(Tokens[Index].Type == '[');

						while(Tokens[Index].Type == '[') {
							do {Index += 1; assert(Index < TokensCount);} while(Tokens[Index].Type != ']');
							Index += 1; assert(Index < TokensCount);
						}
						Index -= 1;
					}

					Index += 1; assert(Index < TokensCount);

					assert(Tokens[Index].Type == ';' || Tokens[Index].Type == ',');
					if(Tokens[Index].Type == ',') {Index += 1; assert(TokenIndex < TokensCount); continue;}
					if(Tokens[Index].Type == ';') {break;}
				} else {
					assert(Tokens[Index].Type == token_IDENTIFIER);
					printf("new type name: %s\n", RangeToString(TempBuffer, Contents, Tokens[Index].Start, Tokens[Index].End));

					Index += 1; assert(TokenIndex < TokensCount);
					while(Tokens[Index].Type == '[') {
						do {Index += 1; assert(TokenIndex < TokensCount);} while(Tokens[Index].Type != ']');
						Index += 1; assert(TokenIndex < TokensCount);
					}

					assert(Tokens[Index].Type == ';' || Tokens[Index].Type == ',');
					if(Tokens[Index].Type == ',') {Index += 1; assert(TokenIndex < TokensCount); continue;}
					if(Tokens[Index].Type == ';') {break;}
				}
			}

			printf("range: %s\n\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));
			Index += 1;
			TokenIndex = Index;
			continue;
		}

		/* variables (including function pointers) and functions */
		if(IsTypeToken(Tokens[TokenIndex].Type)
			|| Tokens[TokenIndex].Type == token_IDENTIFIER
			|| Tokens[TokenIndex].Type == token_KEYWORD_STRUCT
			|| Tokens[TokenIndex].Type == token_KEYWORD_ENUM
			|| Tokens[TokenIndex].Type == token_KEYWORD_UNION) {
			int Index = TokenIndex;

//			Index += 1; assert(Index < TokensCount);

//			while(IsTypeToken(Tokens[Index].Type) || Tokens[Index].Type == token_IDENTIFIER) {
//				Index += 1; assert(TokenIndex < TokensCount);
//			}

			while(Tokens[Index].Type == token_KEYWORD_CONST) {
				Index += 1; assert(TokenIndex < TokensCount);
			}

			if(Tokens[Index].Type == token_KEYWORD_STRUCT
			|| Tokens[Index].Type == token_KEYWORD_ENUM
			|| Tokens[Index].Type == token_KEYWORD_UNION) {
				Index += 1; assert(Index < TokensCount);
				if(Tokens[Index].Type == token_IDENTIFIER) {
					printf("struct, enum or union name: %s\n", RangeToString(TempBuffer, Contents, Tokens[Index].Start, Tokens[Index].End));
					Index += 1; assert(Index < TokensCount);
				} else {
					printf("anonymous struct, enum or union\n");
					assert(Tokens[Index].Type == '{');
				}
				
				if(Tokens[Index].Type == '{') {
					int ScopeCount = 0;
					do {
						Index += 1; if(Index == TokensCount) {break;}
	
						if(Tokens[Index].Type == '{') {
							ScopeCount += 1;
						}
						if(Tokens[Index].Type == '}') {
							if(ScopeCount == 0) {
								break;
							} else {
								ScopeCount -= 1;
							}
						}
					} while(Index < TokensCount);
					assert(Index < TokensCount);
					Index += 1; assert(Index < TokensCount);
				}

				while(Tokens[Index].Type == token_KEYWORD_CONST) {
					Index += 1; assert(Index < TokensCount);
				}

				if(Tokens[Index].Type == ';') {
					// only type, no variables
					Index += 1; assert(Index < TokensCount);
					TokenIndex = Index;
					continue;
				}
			} else {
				while(IsTypeToken(Tokens[Index].Type) || Tokens[Index].Type == token_IDENTIFIER) {
					Index += 1; assert(Index < TokensCount);
				}
			}

			/*
			our possible position: X
			typestuff newtypeX
			typestuff X*newtype
			typestuff newtypeX[]
			typestuff X*newtype[]
			we want to be either at the first '*' or at the new name if no '*'
			typestuff X(*newtype[])(args)
			if func ptr, we want to be at '('
			
			typestuff funcnameX(
			typestuff X*funcname(
			*/
			assert((Index + 1) < TokensCount);
			if(Tokens[Index].Type == ';'
			|| Tokens[Index].Type == ','
			|| Tokens[Index].Type == '='
			|| Tokens[Index].Type == '['
			|| (Tokens[Index].Type == '(' && Tokens[Index + 1].Type != '*')) {
				Index -= 1;
			} else {
//				printf("range debug: %s\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));
				if(Tokens[Index].Type == '(' || Tokens[Index].Type == '*' || Tokens[Index].Type == token_IDENTIFIER){}
				else {
					printf("token type: %c\n", (char)Tokens[Index].Type);
					printf("range: %s\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));
				}
				assert(Tokens[Index].Type == '(' || Tokens[Index].Type == '*' || Tokens[Index].Type == token_IDENTIFIER);
			}

			// type newthing1, newthing2 = initializer, newthing3, ...;
			while(true) {
				while(Tokens[Index].Type == '*' || Tokens[Index].Type == token_KEYWORD_CONST) {
					Index += 1; assert(Index < TokensCount);
				}

				if(Tokens[Index].Type == '(') {
					// func pointer: type (*name) (arguments)
					// or array pointer: type (*name) [arguments]
					Index += 1; assert(Index < TokensCount);

					//@ ?
					while(Tokens[Index].Type == '*' || Tokens[Index].Type == token_KEYWORD_CONST) {
						Index += 1; assert(Index < TokensCount);
					}

					assert(Tokens[Index].Type == token_IDENTIFIER);
					printf("a function pointer or an array pointer name: %s\n", RangeToString(TempBuffer, Contents, Tokens[Index].Start, Tokens[Index].End));
					Index += 1; assert(TokenIndex < TokensCount);

					while(Tokens[Index].Type == '[') {
						do {Index += 1; assert(Index < TokensCount);} while(Tokens[Index].Type != ']');
						Index += 1; assert(Index < TokensCount);
					}

					assert(Tokens[Index].Type == ')');
					Index += 1; assert(Index < TokensCount);

					if(Tokens[Index].Type == '(') {
						// pointer to a function
						do {Index += 1; assert(Index < TokensCount);} while(Tokens[Index].Type != ')');
					} else {
						// pointer to a array
						assert(Tokens[Index].Type == '[');

						while(Tokens[Index].Type == '[') {
							do {Index += 1; assert(Index < TokensCount);} while(Tokens[Index].Type != ']');
							Index += 1; assert(Index < TokensCount);
						}
						Index -= 1;
					}

					Index += 1; assert(Index < TokensCount);

					assert(Tokens[Index].Type == ';' || Tokens[Index].Type == ',' || Tokens[Index].Type == '=');
					/*
					thething = 123,
					thething = 123;
					thething = SomeName,
					thething = SomeName;
					thething = {1,2,3}, nextthing
					thething = {1,2,3};
					?
					*/
					if(Tokens[Index].Type == '=') {
						Index += 1; assert(Index < TokensCount);
						if(Tokens[Index].Type == '{') {
							int ScopeCount = 0;
							do {
								Index += 1; assert(Index < TokensCount);
			
								if(Tokens[Index].Type == '{') {
									ScopeCount += 1;
								}
								if(Tokens[Index].Type == '}') {
									if(ScopeCount == 0) {
										break;
									} else {
										ScopeCount -= 1;
									}
								}
							} while(true);
						}
						do {Index += 1; assert(Index < TokensCount);} while(Tokens[Index].Type != ',' && Tokens[Index].Type != ';');
					}
					if(Tokens[Index].Type == ',') {Index += 1; assert(Index < TokensCount); continue;}
					if(Tokens[Index].Type == ';') {break;}
				} else {
					assert(Tokens[Index].Type == token_IDENTIFIER);
					printf("new type name: %s\n", RangeToString(TempBuffer, Contents, Tokens[Index].Start, Tokens[Index].End));

					Index += 1; assert(TokenIndex < TokensCount);

					// was it a function?
					//@ mess
					if(Tokens[Index].Type == '(') {
						printf("was a function\n");
						do {Index += 1; assert(TokenIndex < TokensCount);} while(Tokens[Index].Type != ')');
						Index += 1; assert(TokenIndex < TokensCount);
						if(Tokens[Index].Type == ';') {
							printf("was a declaration\n");
						} else {
							assert(Tokens[Index].Type == '{');
							printf("was a definition\n");
							int ScopeCount = 0;
							do {
								Index += 1; assert(TokenIndex < TokensCount);
			
								if(Tokens[Index].Type == '{') {
									ScopeCount += 1;
								}
								if(Tokens[Index].Type == '}') {
									if(ScopeCount == 0) {
										break;
									} else {
										ScopeCount -= 1;
									}
								}
							} while(true);
						}
						break;
					}

					while(Tokens[Index].Type == '[') {
						do {Index += 1; assert(TokenIndex < TokensCount);} while(Tokens[Index].Type != ']');
						Index += 1; assert(TokenIndex < TokensCount);
					}

					assert(Tokens[Index].Type == ';' || Tokens[Index].Type == ',' || Tokens[Index].Type == '=');
					/*
					thething = 123,
					thething = 123;
					thething = SomeName,
					thething = SomeName;
					thething = {1,2,3}, nextthing
					thething = {1,2,3};
					?
					*/
					if(Tokens[Index].Type == '=') {
						Index += 1; assert(TokenIndex < TokensCount);
						if(Tokens[Index].Type == '{') {
							int ScopeCount = 0;
							do {
								Index += 1; assert(TokenIndex < TokensCount);
			
								if(Tokens[Index].Type == '{') {
									ScopeCount += 1;
								}
								if(Tokens[Index].Type == '}') {
									if(ScopeCount == 0) {
										break;
									} else {
										ScopeCount -= 1;
									}
								}
							} while(true);
						}
						do {Index += 1; assert(TokenIndex < TokensCount);} while(Tokens[Index].Type != ',' && Tokens[Index].Type != ';');
					}
					if(Tokens[Index].Type == ',') {Index += 1; assert(TokenIndex < TokensCount); continue;}
					if(Tokens[Index].Type == ';') {break;}
				}
			}

			int RangeLength = Tokens[Index].End - Tokens[TokenIndex].Start;
			printf("RangeLength: %d\n", RangeLength);
//			assert((RangeLength + 1) <= sizeof(TempBuffer));
			printf("range: ");
			PrintRange(Contents, Tokens[TokenIndex].Start, Tokens[Index].End);
			printf("\n\n");
			Index += 1;
			TokenIndex = Index;
			continue;
		}

//		printf("unhandled token: %s\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[TokenIndex].End));

		TokenIndex += 1;
	}

	/* parsing */
	/*
		global scope:
		"struct" -> parse struct
		"union" -> parse union
		"enum" -> parse enum
		"typedef" -> parse typedef, but could be typedef followed by struct, enum, union ?
		identifier (static, const, int, float, double, unsigned, signed, short, long, [non-builtin types], ...?) ->
			variable -> parse variable
			function -> parse function
		?
	*/
//	printf("\n*** parsing: ***\n");
//	printf("TokensCount: %d\n", TokensCount);
//	int TokenIndex = 0;
//	while(TokenIndex < TokensCount) {
//		/* struct, enum and union definitions */
//		if(Tokens[TokenIndex].Type == token_KEYWORD_STRUCT) {
//			/*
//				struct {...}; -- generates a warning because nothing is declared/defined
//
//				struct {...} *VarName1, VarName2 = {...}; -- var definitions/declarations
//				struct typeName {...} VarName = {...}; -- type definition and var definition/declaration
//				struct typeName {...} *VarName1, VarName2 = {...}; -- type definition and multiple var definitions/declarations
//
//				struct typeName *VarName1, VarName2 = {...};
//			*/
//			int Index = TokenIndex;
//
//			Index += 1; if(Index == TokensCount) {assert(false);}
////			if(Tokens[Index].Type != token_IDENTIFIER) {assert(false);} //@ typedef struct {}
//			// either anonymous or named struct
//			bool IsAnonymous = true;
//			int StructNameStart;
//			int StructNameEnd;
//			if(Tokens[Index].Type == token_IDENTIFIER) {
//				IsAnonymous = false;
//				StructNameStart = Tokens[Index].Start;
//				StructNameEnd = Tokens[Index].End;
//
//				Index += 1; if(Index == TokensCount) {assert(false);}
//			}
//
//			if(Tokens[Index].Type == '{') {
//				int ScopeCount = 0;
//				do {
//					Index += 1; if(Index == TokensCount) {break;}
//
//					if(Tokens[Index].Type == '{') {
//						ScopeCount += 1;
//					}
//					if(Tokens[Index].Type == '}') {
//						if(ScopeCount == 0) {
//							break;
//						} else {
//							ScopeCount -= 1;
//						}
//					}
//				} while(Index < TokensCount);
//				if(Index == TokensCount) {assert(false);}
//
////				Index += 1; if(Index == TokensCount) {assert(false);}
////				if(Tokens[Index].Type != ';') {assert(false);}
//				//@ Do we have a comma separated list of struct instances?
//				do {	
//					Index += 1;
//				}
//				while(Index < TokensCount && Tokens[Index].Type != ';');
//				if(Index == TokensCount) {assert(false);}
//
//				// unless it's inside a typedef anonymous struct is actually part of a variable definition/declaration, not a type definition...
//				if(IsAnonymous) {
//					strncpy(TempBuffer, "[anonymous]", 11);//@bounds check
//					TempBuffer[11] = '\0';
//				} else {
//					RangeToString(TempBuffer, Contents, StructNameStart, StructNameEnd);
//				}
//				printf("STRUCT DEFINITION (%s):\n", TempBuffer);
//				PrintRange(Contents, Tokens[TokenIndex].Start, Tokens[Index].End);
//				puts("");
//				puts("");
//
//				Index += 1;
//				TokenIndex = Index;
//
//				continue;
//			} else {
//				assert(!IsAnonymous);
//				/*
//				"struct [identifier] [not '{']"
//				it's either a variable definition/declaration,
//				or function definition/declaration
//				*/
//				// fall further into checks below
//				// there should be code that handles these possibilities
//			}
//		}
//
//		/* type definitions */
//		if(Tokens[TokenIndex].Type == token_KEYWORD_TYPEDEF) {
//			int Index = TokenIndex;
//
//			Index += 1; if(Index == TokensCount) {assert(false);}
//			if(Tokens[Index].Type == token_KEYWORD_CONST) {
//				Index += 1; if(Index == TokensCount) {assert(false);}
//			}
//
//			if(Tokens[Index].Type == token_KEYWORD_STRUCT) {
//				Index += 1; if(Index == TokensCount) {assert(false);}
//				// either anonymous or named struct
//				bool IsAnonymous = true;
//				int StructNameStart;
//				int StructNameEnd;
//				if(Tokens[Index].Type == token_IDENTIFIER) {
//					IsAnonymous = false;
//					StructNameStart = Tokens[Index].Start;
//					StructNameEnd = Tokens[Index].End;
//	
//					Index += 1; if(Index == TokensCount) {assert(false);}
//				}
//	
//				if(Tokens[Index].Type == '{') {
//					int ScopeCount = 0;
//					do {
//						Index += 1;
//	
//						if(Tokens[Index].Type == '{') {
//							ScopeCount += 1;
//						}
//						if(Tokens[Index].Type == '}') {
//							if(ScopeCount == 0) {
//								break;
//							} else {
//								ScopeCount -= 1;
//							}
//						}
//					}
//					while(Index < TokensCount);
//					if(Index == TokensCount) {assert(false);}
//	
////					printf("struct definition:\n");
////					PrintRange(Contents, Tokens[TokenIndex].Start, Tokens[Index].End);
////					puts("");
////					puts("");
//	
//					Index += 1;
//	
//	//				printf("defined struct: ");
//	//				if(IsAnonymous) {
//	//					printf("[anonymous]");
//	//				} else {
//	//					PrintRange(Contents, StructNameStart, StructNameEnd);
//	//				}
//	//				puts("");
//				} else {
//					assert(!IsAnonymous);
//	
//	//				printf("simply mentioned: struct name: ");
//	//				PrintRange(Contents, StructNameStart, StructNameEnd);
//	//				puts("");
//				}
//			}
//
//			while(Index < TokensCount && Tokens[Index].Type != ';') {	
//				Index += 1;
//			}
//			if(Index == TokensCount) {assert(false);}
//
//			printf("TYPE DEFINITION (%s): ", RangeToString(TempBuffer, Contents, Tokens[Index - 1].Start, Tokens[Index - 1].End));
//			PrintRange(Contents, Tokens[TokenIndex].Start, Tokens[Index].End);
//			puts("");
//			puts("");
//
//			Index += 1;
//			TokenIndex = Index;
//			
//			continue;
//		}
//
//		/* global variables and functions */
//		if(IsIdentifierGenerallySpeaking(Tokens[TokenIndex].Type)) {
//			/*
//				type:
//					float,
//					unsigned long int const
//					<user type name>
//					struct <struct name> {...} const
//					struct {...} const
//
//				?static
//				(type, including constness) -- type
//				?*... ?const VarName ?[...] -- variable declaration
//					; -> done
//					= expression (1.2, 0xab, {1,2.0,"abc"}, {1,2,3}, 1 + 2 * 3, ...) -- variable definition
//						; -> done
//						, -> another variable definition/declaration
//					, ?*... ?const VarName ?[...]
//
//				-> looking for ';', ',' or '='
//				if ';' -> done ("name;" or "name[...];")
//				if ',' -> expecting another variable declaration/definition
//					back to step 1
//				if '=' -> expecting expression
//			*/
//			int Index = TokenIndex;
//			do {	
//				Index += 1;
//			}
//			while(Index < TokensCount && (IsIdentifierGenerallySpeaking(Tokens[Index].Type) || Tokens[Index].Type == '*' || Tokens[Index].Type == '[' || Tokens[Index].Type == ']'));
//			if(Index == TokensCount) {assert(false);}
//
////			printf("FOUND %s\n\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));
//
//			if(Tokens[Index].Type == ';') {
//				printf("VARIABLE DECLARATION: %s\n\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));
//
//				Index += 1;
//				TokenIndex = Index;
//
//				continue;
//			}
////			if(Tokens[Index].Type == '[') {
////				
////			}
//			if(Tokens[Index].Type == '=') {
//				do {	
//					Index += 1;
//				}
//				while(Index < TokensCount && Tokens[Index].Type != ';');
//				if(Index == TokensCount) {assert(false);}
//
//				printf("VARIABLE DEFINITION: %s\n\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));
//
//				Index += 1;
//				TokenIndex = Index;
//
//				continue;
//			}
//			if(Tokens[Index].Type == '(') {
//				do {	
//					Index += 1;
//				}
//				while(Index < TokensCount && Tokens[Index].Type != ')');
//				if(Index == TokensCount) {assert(false);}
//
//				int FunctionHeaderStart = TokenIndex;
//				int FunctionHeaderEnd = Index;
//
//				Index += 1; if(Index == TokensCount) {assert(false);}
//				if(Tokens[Index].Type == ';') {
//					printf("FUNCTION DECLARATION: %s\n\n", RangeToString(TempBuffer, Contents, Tokens[FunctionHeaderStart].Start, Tokens[FunctionHeaderEnd].End));
//
//					Index += 1;
//					TokenIndex = Index;
//
//					continue;
//				} else {
////					assert(Tokens[Index].Type == '{');
//					if(Tokens[Index].Type != '{') {printf("FAILED: %s\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));}
//
//					int ScopeCount = 0;
//					do {
//						Index += 1; if(Index == TokensCount) {break;}
//
//						if(Tokens[Index].Type == '{') {
//							ScopeCount += 1;
//						}
//						if(Tokens[Index].Type == '}') {
//							if(ScopeCount == 0) {
//								break;
//							} else {
//								ScopeCount -= 1;
//							}
//						}
//					} while(Index < TokensCount);
//					if(Index == TokensCount) {assert(false);}
//
//					printf("FUNCTION DEFINITION: %s\n\n", RangeToString(TempBuffer, Contents, Tokens[FunctionHeaderStart].Start, Tokens[FunctionHeaderEnd].End));
//
//					Index += 1;
//					TokenIndex = Index;
//
//					continue;
//				}
//			}
//
////			if(Tokens[Index].Type == '{') {
////				printf("FUNCTION DEFINITION: %s\n\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));
////			} else {
////				printf("UNKNOWN: %s\n\n", RangeToString(TempBuffer, Contents, Tokens[TokenIndex].Start, Tokens[Index].End));
////			}
//
//			Index += 1;
//			TokenIndex = Index;
//
//			continue;
//		}
//
//		TokenIndex += 1;
//	}

//	/* searching */
//	{
//	printf("\n*** patterns: ***\n");
//	int TokenIndex = 0;
//	while(TokenIndex < TokensCount) {
////		if(
////		Tokens[TokenIndex].Type == token_IDENTIFIER
////		&& Tokens[TokenIndex + 1].Type == token_IDENTIFIER
////		&& Tokens[TokenIndex + 2].Type == '('
////		) {
////			int PatternLength = Tokens[TokenIndex + 2].End - Tokens[TokenIndex].Start;
////			strncpy(TempBuffer, &Contents[Tokens[TokenIndex].Start], PatternLength);//@bounds
////			TempBuffer[PatternLength] = '\0';
////
////			printf("found pattern: %s\n", TempBuffer);
////
////			TokenIndex += 3;
////			continue;
////		}
//
//		/*
//		function definition:
//			identifier identifier (
//
//		return_type function_name ( any_number_of_arguments )
//		[static] identifier [*...] identifier ( )
//
//		any number '*' and identifiers followed by '('
//		*/
//		if(Tokens[TokenIndex].Type == token_IDENTIFIER) {
//			int Index = TokenIndex + 1;
//			while(Index < TokensCount) {
//				if(Tokens[Index].Type != token_IDENTIFIER && Tokens[Index].Type != '*') {break;}
//
//				Index += 1;
//			}
//			if(Index == TokensCount) {break;}
//
//			/*
//			variable def/dec: 2 or more identifiers (what about '*'?)
//			const int *Ptr;
//			const unsigned int * const Ptr;
//			unsigned int const ** const Ptr;
//			*/
//
//			int Count = Index - TokenIndex;
//			if(Count > 1 && Tokens[Index].Type == '(') {
////				printf("function name: ");
////				PrintRange(Contents, Tokens[Index - 1].Start, Tokens[Index - 1].End);
////				puts("");
//				
//				// parse function arguments
//				/*
//				... func ()
//				... func (void)
//				... func (int a, unsigned int b)
//				*/
//				Index += 1;
//				while(Index < TokensCount) {
////					int StartArgument = Index;
//					while(Index < TokensCount) {
//						if(Tokens[Index].Type != token_IDENTIFIER && Tokens[Index].Type != '*') {break;}
//
//						Index += 1;
//					}
//					if(Index == TokensCount) {break;}
////					int EndArgument = Index - 1;
//
////					if(EndArgument - StartArgument < 0) {
////						printf("no arguments\n");
////					} else {
////						printf("argument: ");
////						PrintRange(Contents, Tokens[StartArgument].Start, Tokens[EndArgument].End);
////						puts("");
////					}
//
//					if(Tokens[Index].Type == ')') {break;}
//	
//					Index += 1;
//				}
//				if(Index == TokensCount) {break;}
//
//				Index += 1;
//				if(Index == TokensCount) {break;}
//				if(Tokens[Index].Type == '{') {
//					int PatternLength = Tokens[Index].End - Tokens[TokenIndex].Start;
//					strncpy(TempBuffer, &Contents[Tokens[TokenIndex].Start], PatternLength);//@bounds
//					TempBuffer[PatternLength] = '\0';
//	
//					printf("found pattern: %s\n", TempBuffer);
//	
//					int CountTokens = Index - TokenIndex + 1;
//					printf("-> %d\n", CountTokens);
//					TokenIndex += CountTokens;
//					continue;
//				}
//			}
//		}
//
//		TokenIndex += 1;
//	}
//	}

	if(munmap(Contents, FileSize) == -1){
		fprintf(stderr, "error: unmapping the file\n");
		return 1;
	}
	printf("successfully unmapped the file\n");

	return 0;
}


