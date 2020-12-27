#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define MAXLINE 100
#define MAX_LISTEN_QUEUE 100

//////////////////////////////////////// Account ////////////////////////////////////////
typedef struct node {
  	char username[20];
	char password[20];
	int status;
	struct node *next;
}node;

node *head = NULL;

void insert(char username[20], char password[20], int status){
	node *temp;
	temp=(node *)malloc(sizeof(node));
	strcpy(temp->username, username);
	strcpy(temp->password, password);
	temp->status = status;
	temp->next = head;
	head = temp;
}

node *find(char name[20]){
   node *current = head;
   if(head == NULL)
	{
      return NULL;
    }
   while(strcmp(current->username, name) != 0){
      if(current->next == NULL){
         return NULL;
      }else {
         current = current->next;
	
      }
   }      
   return current;
}

void printAll()
{
    node *temp;
    temp = head;
    while(temp)
    {
    	printf("%20s %20s %d",temp->username, temp->password, temp->status);
        printf("\n");
        temp=temp->next;
    }
    printf("\n");
}

void openFile(){
	node *acc;
	char *username;
	char *password;
	int status;
	username = (char *)malloc(20);
	password = (char *)malloc(20);
    FILE *fptr;
	if((fptr=fopen("account.txt","r+"))==NULL){
		printf("Not find%s\n","account.txt");
		return;
	}
	while(1){
		fscanf(fptr,"%s",username);
		fscanf(fptr,"%s",password);
		fscanf(fptr,"%d",&status);
		if(feof(fptr)) break;
		insert(username, password, status);
	}
	free(username); free(password);
	fclose(fptr);
}

void writeFile(){
	FILE *fptr;
	node *temp;
    temp = head;
    fptr=fopen("account.txt","w+");
    while(temp){
    	fprintf(fptr, "%s %s %d", temp->username, temp->password, temp->status);
    	fprintf(fptr, "\n");
    	temp=temp->next;
    }
    fclose(fptr);
}

void registerAccount(){
	char username[20];
	char password[20];
	char homepage[200];
	printf("---------Register\n");
	printf("Username: "); scanf("%s", username);
	printf("\n");
	printf("Password: "); scanf("%s", password);
	printf("\n");
	
	if(find(username) != NULL){
	printf("Account existed!");	
	} 
	else{
		insert(username, password, 1);
		writeFile();
		printf("Successful registration. Activation required.\n");
	}
	printf("\n");
}

void activateAccount(){
    char username[20];
	char password[20];
    char temp[20];
    char activationCode[20] = "LTM121216";
    int count = 0;
    printf("---------Activate\n");
    printf("Type username: "); scanf("%s", username);
    printf("\n");
    node *acc = find(username);
    if(acc != NULL){
		if(acc->status == 0){
			printf("Account has been blocked!\n");
            printf("------------------------\n");
		}
        else if(acc->status == 1){
            printf("Account is already activated!\n");
            printf("------------------------\n");
        }
        else if(acc->status == 2){
            printf("Account is idle - need to be activated!\n");
            printf("Type password: "); scanf("%s", password);
            printf("\n");
            if (strcmp(acc->password,password) == 0){
			printf("Success!\n");
            while (count<6) {
            printf("Type activation code: "); scanf("%s", temp);
            printf("\n");
            if (strcmp(temp,activationCode) == 0) {
                acc->status = 1;
                printf("Successful activation.\n");
                printf("------------------------\n");
                writeFile();
                printAll();
                count = 6;
                }
                else { 
                    count++;
                    if (count == 5) {
                        printf("Wrong code! You have failed 5 times. This account is now blocked!\n");
                        printf("------------------------\n");
                        acc->status = 0;
                        writeFile();
                        printAll();
                        count++;
                        }  
                    else {
                        printf("Wrong code! Try again.\n");
                    }
			}   
            }
        }
        else {
                printf("Wrong password!");
                printf("------------------------\n");
            }
        }
    }
    else { 
            printf("Account doesn't exist!\n");
            printf("------------------------\n");
           }
}

node *login(){
    char username[20];
	char password[20];
	int  count = 0;
    printf("---------Login\n");
    printf("Type username: "); scanf("%s",username);
    printf("\n");
    node *acc = find(username);
    if (acc != NULL) {
        if (acc->status == 0) {
            printf("Account is blocked!");
            printf("------------------------\n");
            return NULL;
        }
        else if (acc->status == 2) {
            printf("Account needs to be activated!");
            printf("------------------------\n");
            return NULL;
        }
        else {
            while (count<4) {
                printf("Type password: "); scanf("%s",password);
                printf("\n");
                if (strcmp(acc->password,password) == 0) {
                    printf("Login succesfully.\n");
                    printf("------------------------\n");
                    return acc;
                }
                else {
                    count++;
                    if (count==3) {
                        printf("Wrong password! You have failed 3 times. This account is now blocked!\n");
                        printf("------------------------\n");
                        acc->status = 0;
                        writeFile();
                        return NULL;
                    }
                    else {
                        printf("Wrong password! Try again.\n");
                    }
                }
            }
        }
    }
    else {
        printf("Account doesn't exist!");
        printf("------------------------\n");
        return NULL;
    }
}

void search(){
	char username[20];
	printf("---------Search\n");
	printf("Type username: "); scanf("%s", username);
	printf("\n");
	node *acc = find(username);
	if(acc != NULL){
		printf("Username: %20s Status: %d\n", acc->username, acc->status);
        printf("------------------------\n");
	}
	else {
        printf("Account does not exist\n");
        printf("------------------------\n");
        }
}

node *signout(){
	char username[20];
	printf("---------Sign Out\n");
	printf("Username: "); scanf("%s", username);
	printf("\n");
	node *acc = find(username);
	if(acc == NULL){
		printf("Account does not exist\n");
		return NULL;
	}
	else return acc;
}
//////////////////////////////////////// Account ////////////////////////////////////////
//////////////////////////////////////// Question ////////////////////////////////////////
typedef struct bank{
    char question[120];
    char choiceA[70];
    char choiceB[70];
    char answer[5];
    char aPoint[5];
    char bPoint[5];
}Bank;
 
typedef struct question
{
    char word[1000];
    char answer[5];
    char aPoint[5];
    char bPoint[5];
}Question;

Bank setQuestion[20];
Question questionList[20];
FILE *question;

void readQuestion(){ // Gets and stores questions
    if((question = fopen("question.txt", "r"))==NULL){
        printf("File doesn't exist!\n");
		return;
    }
    else{
        while(!feof(question)){
            for(int i=0; i<75; i++){ // Reads the questions from file
                fgets(setQuestion[i].question, 120, question);
                fgets(setQuestion[i].choiceA, 70, question);
                fgets(setQuestion[i].choiceB, 70, question);
                fgets(setQuestion[i].answer, 70, question);
                fgets(setQuestion[i].aPoint, 70, question);
                fgets(setQuestion[i].bPoint, 70, question);
            }
        }
    }
    fclose(question);
}

void makeQuestion(){
    Question *buff;
    readQuestion();
    for (int i = 0; i < 75; i++)
    {
        buff = (Question *)malloc(sizeof(Question));
        strcpy((*buff).word, setQuestion[i].question);
        strcat((*buff).word, setQuestion[i].choiceA);
        strcat((*buff).word, setQuestion[i].choiceB);
		(*buff).word[strlen((*buff).word) - 1] = '\0';
        strcpy((*buff).answer, setQuestion[i].answer);
		(*buff).answer[strlen((*buff).answer) - 1] = '\0';
        strcpy((*buff).aPoint, setQuestion[i].aPoint);
        // (*buff).aPoint[strlen((*buff).aPoint) - 1] = '\0';
        strcpy((*buff).bPoint, setQuestion[i].bPoint);
        // (*buff).bPoint[strlen((*buff).bPoint) - 1] = '\0';
        strcpy(questionList[i].word, (*buff).word);
        strcpy(questionList[i].answer, (*buff).answer);
        strcpy(questionList[i].aPoint, (*buff).aPoint);
        strcpy(questionList[i].bPoint, (*buff).bPoint);
        free(buff);
    }
}

void swap(Question *a, Question *b) { 
    Question temp = *a; 
    *a = *b; 
    *b = temp; 
} 

void randomize(Question arr[], int n) { 
    srand(time(NULL)); 

    for (int i = n-1; i > 0; i--) 
    { 
        int j = rand() % (i+1); 
        swap(&arr[i], &arr[j]); 
    } 
}

// void convertAnswer(char a[5]) {

// }
//////////////////////////////////////// Question ////////////////////////////////////////
void sendMess(char *content, int sockfd, struct sockaddr *servaddr){
	int len, sendBytes;
	  
	len = sizeof(*servaddr);

	sendBytes = send(sockfd, content, strlen(content) +1, 0);

	if(sendBytes < 0){
		perror("Error: \n");
	}
	
}

pid_t fork(void);

int main(int argc, char* argv[]){


	pid_t pid;
    int listenfd, connfd, n, portNumber;
    pid_t childpid;
    socklen_t clilen;
    char buff[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;

    portNumber = atoi(argv[1]);


    // construct socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // bind socket to ip address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portNumber);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, MAX_LISTEN_QUEUE);
    printf("%s\n", "Server running...waiting for connections.");

    makeQuestion();
    node *acc;
	int loginVar = 0, optionVar = 0, i = 0, readyCheck = 0;
    int count = 0, tempAPoint = 0, tempBPoint = 0;
    char username[20], password[20], tempPassword[20], tempUsername[20];
	
    // communicate with client
    while(1){
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        printf("%s\n", "Received request...");

		if( (pid = fork()) == 0) {           //process in child (forking)
			
			close(listenfd); 	             // child closes listening socket
			while(n = recv(connfd, buff, MAXLINE, 0) > 0){
                openFile();
                if (strcmp(buff,"1") == 0 && loginVar == 0) {
                    loginVar = 1;
                    sendMess("--- Nhập tên tài khoản mới ---", connfd, (struct sockaddr*) &cliaddr);
                }
                else if (strcmp(buff,"2") == 0 && loginVar == 0) {
                    sendMess("--- Nhập tên tài khoản của bạn ---", connfd, (struct sockaddr*) &cliaddr);
                    loginVar = 3;
                }
                else {
                    switch (loginVar) {
                        case 1: 
                        if (find(buff) == NULL) {
                            strcpy(tempUsername,buff);
                            sendMess("--- Nhập mật khẩu ---", connfd, (struct sockaddr*) &cliaddr);
                            strcpy (username, buff);
                            loginVar = 2;
                        }
                        else {
                            sendMess("--- Tài khoản đã tồn tại ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 0;
                        }
                        break;

                        case 2:
                        sendMess("--- Tạo tài khoản thành công ---", connfd, (struct sockaddr*) &cliaddr);
                        strcpy (password, buff);
                        insert (username,password,1);
                        writeFile();
                        loginVar = 5;
                        acc = find(tempUsername);
                        break;

                        case 3:
                        acc = find(buff);
                        if (acc == NULL) {
                            sendMess("--- Tài khoản không tồn tại ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 0;
                        }
                        else {
                            sendMess("--- Nhập mật khẩu ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 4;
                        }
                        break;

                        case 4:
                        if (strcmp(acc->password,buff) == 0) {
                            sendMess("--- Đăng nhập thành công ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 5;
                        }
                        else {
                            sendMess("--- Mật khẩu không chính xác ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 0;
                        }
                        break;

                        case 5:
                        if (strcmp(buff,"2") == 0 && optionVar == 0) {
                            sendMess("--- Nhập mật khẩu mới ---", connfd, (struct sockaddr*) &cliaddr);
                            optionVar = 2;
                        }
                        else if (strcmp(buff,"3") == 0 && optionVar == 0) {
                            sendMess("--- Đăng xuất thành công ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 0;
                            optionVar = 0;
                        }
                        else if (strcmp(buff,"1") == 0 && optionVar == 0) {
                            sendMess("Nhập READY nếu bạn đã sẵn sàng", connfd, (struct sockaddr*) &cliaddr);
                            optionVar = 1;
                        }
                        else {
                            switch (optionVar) {
                            case 1:
                            printf("%s",buff);
                            if (strcmp(buff,"READY") == 0) {
                                randomize(questionList,3);
                                sendMess(questionList[i].word, connfd, (struct sockaddr*) &cliaddr);
                                readyCheck = 1;
                            }
                            else if ((strcmp(buff,"A") == 0 || strcmp(buff,"a") == 0) && readyCheck == 1) {
                                tempAPoint = atoi(questionList[i].aPoint);
                                count = count + tempAPoint;
                                i = i + 1;
                                if (i < 20) {
                                    sendMess(questionList[i].word, connfd, (struct sockaddr*) &cliaddr);
                                }
                                else {
                                    sendMess("Bạn đã trả lời xong, vui lòng nhập OK để nhận đáp án từ hệ thống", connfd, (struct sockaddr*) &cliaddr);
                                    i = 0;
                                    optionVar = 4;
                                }
                            }
                            else if ((strcmp(buff,"B") == 0 || strcmp(buff,"b") == 0) && readyCheck == 1) {
                                tempBPoint = atoi(questionList[i].bPoint);
                                count = count + tempBPoint;
                                i = i + 1;
                                if (i < 20) {
                                    sendMess(questionList[i].word, connfd, (struct sockaddr*) &cliaddr);
                                }
                                else {
                                    sendMess("Bạn đã trả lời xong, vui lòng nhập OK để nhận đáp án từ hệ thống", connfd, (struct sockaddr*) &cliaddr); 
                                    i = 0;
                                    optionVar = 4;
                                }
                            }
                            else {
                                sendMess("--- Nhập sai rồi!!!! ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                i = 0;
                                count = 0;
                            }

                            
                            break;

                            case 2:
                            strcpy(tempPassword,buff);
                            sendMess("--- Nhập lại mật khẩu mới ---", connfd, (struct sockaddr*) &cliaddr);
                            optionVar = 3;
                            break;

                            case 3:
                            if (strcmp(tempPassword,buff) == 0) {
                                strcpy(acc->password,tempPassword);
                                writeFile();
                                sendMess("--- Đổi mật khẩu thành công ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                }
                            else {
                                sendMess("--- Mật khẩu không khớp ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                }
                            break;

                            case 4:
                            if (strcmp(buff,"OK") == 0) {
                                if (count == 6) {
                                    sendMess("Bạn thuộc nhóm người ENFJ (Người chỉ dạy - The Teacher).\nENFJ là những người giàu năng lượng, có định hướng và rất nhiều khả năng sâu bên trong. Họ có khả năng tự điều chỉnh theo nhu cầu của nhiều người khác trong tập thể. ENFJ luôn muốn kết nối chặt chẽ với mọi người, hỗ trợ và hợp tác với họ trong công việc. ENFJ cũng rất tham vọng, nhưng tham vọng của họ không phải cá nhân mà luôn hướng tới cộng đồng để giúp mọi người cùng tốt hơn.\n=> Phù hợp với các công việc có thiên hướng giúp đỡ người khác như là giáo viên, tư vấn viên sức khỏe, nhà tâm lý học,...", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ESFJ (Người quan tâm - The Provider).\nNhóm tính cách này là những người thực tế, vị tha, giỏi làm việc nhóm, truyền thống và làm hết sức mình để hỗ trợ và bảo vệ lẽ phải và quyền lợi của họ, vì vậy họ có xu hướng rất tận tụy ngay cả khi họ đóng vai trò là người chủ trì của một bữa tiệc hoặc một nhân viên xã hội.\n=> Phù hợp với các công việc giúp đỡ người khác theo những cách thực tế như giáo dục sức khỏe, tư vấn viên, huấn luyện viên cá nhân, giáo viên, y tá, bác sĩ,...", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ESFP (Người trình diễn - The Performer).\nBạnthích là trung tâm của sự chú ý và cũng thích những điều đơn giản nhất. Sự vui vẻ và bản chất nồng nhiệt của bạn thường rất hấp dẫn người khác, vì vậy bạn không bao giờ cạn ý tưởng và sự tò mò của bạn là vô hạn.\n=> Phù hợp với các công việc có tính nghệ thuật, môi trường làm việc xã hội năng động, linh hoạt, vui vẻ như là biên tập viên, ca sĩ, nhạc sĩ, thiết kế thời trang,...", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ESTJ (Người giám sát - The Supervisor).\nESTJ là những người lễ nghi, thực tế, chăm chỉ và trách nhiệm cao với một bộ óc nhạy bén trong việc kinh doanh, tổ chức quản lý cũng như con mắt nhìn người. Họ làm việc có trật tự, quy tắc và một hệ thống phương pháp rõ ràng.\n=> Phù hợp với các công việc quản lý để có thể đưa ra những quyết định về chính sách, thủ tục", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người INFP (Người duy tâm - The Healer).\nINFP thường rất nhạy cảm, đồng cảm, có lòng vị tha và quan tâm đến sự phát triển của bản thân cũng như những người khác, và mong đợi mọi người cũng làm như vậy. Mang tư tưởng cá nhân nhưng cũng rất sáng tạo, linh hoạt và có hơi hướng nghệ thuật, và không hề phán xét trong việc đối xử với người khác.\n=> Phù hợp với các công việc tự chủ, sáng tạo, phù hợp với các giá trị cá nhân của họ để giúp đỡ, cải thiện hoàn cảnh cho người khác (chăm sóc sức khỏe, tâm lý học, xã hội học,...)", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ISFP (Người nghệ sĩ - The Composer).\nISFP là những người hòa bình, thân thiện, dễ tính. Với phương châm sống là để tận hưởng những khoảnh khắc của cuộc sống, họ luôn lạc quan, vui vẻ và linh hoạt một cách tự phát với dòng chảy  để tận hưởng những gì cuộc sống mang lại. ISFP có tính thẩm mỹ cao, luôn tìm kiếm cái đẹp, họ đặc biệt xuất sắc trong việc sử dụng sự sáng tạo, tài năng tự nhiên trong nghệ thuật.\n=> Phù hợp với các công việc có môi trường làm việc hợp tác, lịch sự nơi họ có thể làm việc một cách yên tĩnh với sự hỗ trợ khi cần thiết (ca sĩ, nhạc sĩ, thiết kế thời trang, kiến trúc sư, thợ mộc, thợ may,...)", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người INFJ (Người cố vấn - The Counselors).\nINFJ là những người rất tận tâm và có định hướng rõ ràng, họ luôn tìm kiếm các ý nghĩa trong các mối quan hệ, ý tưởng và các sự kiện để mong muốn hiểu được bản thân và những người xung quanh. Bằng các kỹ năng trực quan, tầm nhìn và sự tự tin để phát triển cải thiện cuộc sống của mọi người.\n=> INFJ chú tâm đến điều mà họ thích làm nhất, đó là việc cải thiện tình trạng con người. Họ thích lập kế hoặch, tổ chức có quy trình, sáng tạo trong môi trường độc lập để thực hiện các sứ mệnh nhân đạo (giáo dục sức khỏe, tư vấn viên, nhà tâm lý học,...)", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ISFJ (Người che chở - The Protector).\nISFJ thật sự đề cao đến việc duy trì trật tự và sự hài hòa trong mọi khía cạnh của cuộc sống, họ là những người kiên định, tỉ mỉ và có trách nhiệm trong công việc. Mặc dù là người sống hướng nội nhưng họ rất giỏi trong việc quan sát và định hướng con người, họ không chỉ nhớ rõ các chi tiết của những người xung quanh mà con nhạy bén trong việc theo dõi cảm xúc của người khác.\n=> ISFJ chú tâm đến điều mà họ thích làm nhất, đó là việc giúp đỡ người khác theo những cách thực tế, có tổ chức, có sự theo dõi quan sát và xem kết quả (giáo dục sức khỏe, tư vấn viên, luật sư, y tá,...)", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ESTP (Người đề xướng - The Promoter).\nESTP là những người tích cực, có xu hướng thực hành để giải quyết vấn đề hơn là chỉ thảo luận về chúng.  ESTP thích tập trung vào các kết quả rõ ràng trước mắt hơn là suy nghĩ về những mục tiêu dài hạn. Họ có khả năng tháo nút thắt cho các vấn đề để mang người khác đồng hành với mình qua đó đem lại những điều tốt đẹp cho cuộc sống. Vì thế mà ESTP được gọi là những Promoter – Người đề xướng.\n=> ESTP là người có động lực với các vấn đề mang tính logic, họ thường chọn nghề nghiệp liên quan tới kỹ năng cơ khí, thể thao hoặc đàm phán đối ngoại. ESTP phù hợp với các nhóm ngành sau : Khoa học và đời sống, giáo dục, kinh doanh bán hàng tài chính, bảo trì và sửa chữa, sản xuất, kiến trúc sư, cảnh sát vệ sĩ bảo vệ, xây dựng,...", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ENFP (Người dẫn dắt thành công - The Champion).\nENFP là những người sáng tạo, giàu năng lượng, đam mê  hứng thú với mọi người và khả năng của họ. ENFP luôn mang đến những ý tưởng, sinh lực và kích thích người khác phát triển bằng sự nhiệt tình trong những hơi thở của họ.  ENFP thích sự mới lạ, luôn lạc quan, hang hái và cảm xúc mạnh mẽ. Ho khát khao dành chiến thắng và chia sẻ kinh nghiệm của họ đến những người khác.\n=> Đúng với tính cách của một ENFP, khi họ tìm kiếm một công việc, họ luôn chú tâm đến điều mà họ thích làm nhất, đó là dùng óc sáng tạo của mình để mang lại lợi ích cho mọi người. Phù hợp với các nhóm ngành như sau: Nghệ thuật và giải trí, kinh doanh và bán hàng, khoa học(tâm lý học, xã hội học), giáo dục, chăm sóc sức khoẻ, lập trình viên :D,..", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ISTJ(Người kiểm duyệt - The Inspector.\nISTJ là mẫu người của sự logic, có tổ chức, hợp lý và nghiêm túc, thích giữ cho cuộc sống và môi trường làm việc của họ được điều tiết tốt. Họ sống khá dè dặt và nghiêm nghị, thành công đến với họ nhờ sự thấu đáo và đáng tin cậy. ISTJ sống rất thực tế và luôn có trách nhiệm. Là dạng người sống hướng nội nên cũng dễ hiểu khi các ISTJ gặp khó khăn trong việc đáp ứng nhu cầu chia sẻ cảm xúc với mọi người xung quanh nhưng không phải là dạng cô lập hoặc tách biệt hẳn với người khác. Nhìn chung, ISTJ là dạng người truyền thống, quy tắc, luôn quan tâm đến việc duy trì trật tự xã hội và các tiêu chuẩn.\nISTJ là những người tỉ mỉ, chi tiết và đáng tin tưởng trong công việc. Một công việc lý tưởng cho ISTJ phải đảm bảo các yếu tố logic, quy tắc rõ ràng. ISTJ phù hợp với các nhóm ngành sau: Dịch vụ cộng đồng và xã hội, giáo dục, pháp luật, bảo trì và sửa chữa, kiến trúc sư, lập trình viên, quản lý, văn phòng hành chính, kinh doanh bán hàng và tài chính,...", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ISTP (Người thợ thủ công - The Crafter).\nISTP rất giỏi về các vấn đề cơ khí, sửa chữa, khắc phục sự cố và có xu hướng nghiên cứu cách mọi thứ hoạt động. Họ độc lập, logic, linh hoạt và dễ thích nghi, vì vậy họ rất giỏi ứng phó và tìm kiếm các giải pháp thực tế cho các vấn đề trong tầm tay cũng như các tình huống khẩn cấp. ISTP làm việc khá độc lập, tách rời và có phần khép kín, họ muốn được làm việc một cách tự do trong một không gian cá nhân để có thể đi theo sự dẫn dắt của chính bản thân mình.\nISTP là dạng người thích làm việc trong môi trường chuyên môn kỹ thuật, đam mê làm chủ và điều khiển các công cụ, thiết bị cơ khí hoặc các công cụ kinh doanh, công nghệ hiện đại. Phù hợp với các nhóm ngành sau đây: Phân tích tài chính, chứng khoán, nhà kinh tế, nhà sinh vật học - địa chất học, cảnh sát - sỹ quan, dịch vụ sức khoẻ - chăm sóc cá nhân(PT, bảo mẫu, y tá,...), kiến trúc sư - thợ mộc - thợ may - đầu bếp, phi công - thuỷ thủ, IT,..", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người INTJ (Người quân sư - The Mastermind).\nINTJ có một niềm đam mê mạnh mẽ với việc phân tích, giải quyết vấn đề, cải thiện các hệ thống và quy trình bằng các ý tưởng sáng tạo của họ. Sự độc lập trong tính cách và tư tưởng đôi khi khiến INTJ bị xa cách nhưng họ luôn là những đối tác trung thành. INTJ nhận thức về các hệ thống và lên chiến lược cho nó, và thông thường thì mọi thứ sẽ được định hướng rõ ràng như trên một bàn cờ. Với khả năng tư duy logic, sự tự tin và tài năng, The Mastermind hoàn toàn có thể đảm nhận vai trò lãnh đạo, cố vấn chiến lược tài ba.\nINTJ vượt trội trong việc tạo ra và thực hiện các giải pháp sáng tạo cho các vấn đề cần phải phân tích. Họ thường thích làm việc độc lập hoặc một nhóm nhỏ, thực hiện các bước chiến lược và đo lường để tạo ra thay đổi. Phù hợp với các nhóm ngành sau: kinh doanh - tài chính, toán học, kiến trúc - kỹ thuật, nghệ thuật - truyền thông, chăm sóc sức khoẻ, giải trí và thể thao, IT, pháp luật,..", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người INTP (Người kiến trúc sư - The Architect).\nINTP là những cá nhân trầm tính, chu đáo, có niềm đam mê mãnh liệt với phân tích logic, hệ thống phức tạp và thiết kế. Họ thường tò mò và thích tìm hiểu về các hệ thống, lý thuyết phức tạp và cách mọi thứ hoạt động. INTP thông thường không phải kiểu người truyền thống, họ có những suy nghĩ riêng và không chạy theo đám đông.\nINTP là người hoạt động độc lập tốt hoặc một nhóm nhỏ đồng nghiệp có năng lực, thông minh và logic. Họ thích một môi trường linh hoạt, cho phép họ xử lý các vấn đề một cách sáng tạo, không bị ràng buộc bởi các quy tắc truyền thống. Họ phù hợp với các nhóm ngành sau đây: IT, kỹ sư kỹ thuật, khoa học, kinh doanh - tài chính, giải trí - nghệ thuật(nhiếp ảnh giam, biên tập viên, nhạc sĩ,..),..", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else if (count == 2) {
                                    sendMess("Bạn thuộc nhóm người ENTP (Người phát minh - The Inventor).\nENTP được mô tả là những người thông minh, lanh lợi, hướng ngoại, sáng tạo, linh hoạt và tháo vát. Họ luôn xuất sắc trong việc đưa ra các giải pháp mới mẻ và độc đáo trước các vấn đề khó khăn, thường thì họ không lên kế hoặch trước cho một dự án. ENTP cũng có một sự hiểu biết sâu sắc về cách mà mọi thứ cũng như các mối quan hệ hoạt động và làm cách nào để cải thiện chúng.\nENTP là người quan tâm đến việc áp dụng các giải pháp sáng tạo cho các vấn đề thách thức trong công việc nhằm mang lại hiệu quả tối ưu nhất có thể. Một môi trường luôn tạo sự thách thức trí tuệ, đòi hỏi liên tục sự nâng cao kiến thức cùng với những người đồng nghiệp thông minh là ưa thích đối với ENTP. Họ phù hợp với các nhóm ngành sau đây: Kinh doanh - lãnh đạo(quản lý kinh doanh, quản trị nhân sự, CEO,..), nghệ thuật - thiết kế(kiến trúc sư, sản xuất nghệ thuật,..), khoa học kỹ thuật,..", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                                else {
                                    sendMess("Bạn thuộc nhóm người ENTJ (Người thống lĩnh - The Field Marshal).\nENTJ có tố chức để trở thành một nhà lãnh đạo, thiết lập chiến lược và tạo ra động lực cho tổ chức đạt được sự tiến bộ. Họ nổi bật trong việc phân tích, lý luận logic và đầu óc tư duy xuất sắc. Ngoài ra, họ còn rất quyết đoán, bộc trực và luôn thích chịu trách nhiệm khi phát sinh vấn đề lỗi. ENTJ tự tin về khả năng của mình, áp đảo người khác bởi đầu óc lập luận sắc bén và ngôn từ sắc sảo. Tuy nhiên họ cũng rất khách quan và không bị ảnh hưởng bởi các vấn đề xung đột hoặc sự chỉ trích xung quanh. Đôi lúc, một vài người có thể cho rằng ENTJ kiêu ngạo, vô cảm nhưng đó là bản năng của họ, suy cho cùng thì họ vẫn muốn tập trung vào kết quả của công việc với năng suất cao nhất.\nENTJ mong muốn ngồi vào các vị trí lãnh đạo nơi mà cho họ đủ quyền lực để thực thi các chiến lược của mình. Một môi trường được cấu trúc bởi những người có năng lượng, tư duy và thông minh là lý tưởng cho ENTJ. Họ phù hợp với các nhóm ngành sau đây: kinh doanh - tài chính, lãnh đạo - quản lý(quản lý nhân sự, quản trị hội đồng), kiến trúc - kỹ thuật, khoa học - đời sống, nghệ thuật - truyền thông, giáo dục, giải trí - thể thao, IT, pháp luật,...", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                    count = 0;
                                }
                            }
                            else {
                                sendMess("--- Nhập sai rồi!!!! ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                readyCheck = 0;
                                count = 0;
                            }
                            break;

                            case 5:
                            if (strcmp(buff,"OK") == 0){
                                sendMess("--- Cảm ơn bạn đã tham gia chơi ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                readyCheck = 0;
                            }
                            else {
                                sendMess("--- Cảm ơn bạn đã tham gia chơi ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                readyCheck = 0;
                            }
                            break;

                            default:
                            sendMess("--- Nhập sai rồi!!!! ---", connfd, (struct sockaddr*) &cliaddr);
                            break;
                            }
                            
                        }

                        default:
                        sendMess("--- Vui lòng chọn lại ---", connfd, (struct sockaddr*) &cliaddr);
                        break;
                    }
                }
        	}

			exit(0);			
		
		}

        if(n < 0){
            perror("Read error: ");
            exit(-1);
        }

        close(connfd);
    }

    close(listenfd);

    return 0;
}