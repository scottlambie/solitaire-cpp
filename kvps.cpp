#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

struct card
{
    // This struct represents a card
    std::string suit;
    int number;
    bool visible;
};

class PrintVectors
{
public:
    void printVector(std::vector<card> vec)
    {
        int i = 0;
        for (auto it = begin(vec); it != end(vec); ++it)
        {
            i++;
            if ((*it).visible == true)
            {
                std::cout << (*it).suit << (*it).number << " ";
            }
            else
            {
                std::cout << "X ";
            }
            if (i % 13 == 0)
            {
                std::cout << "\n";
            }
        }
        std::cout << "\n";
    }

    void printMultipleVectors(std::vector<std::vector<card>> vecs)
    {
        int i = 0;
        for (auto it = begin(vecs); it != end(vecs); ++it)
        {
            ++i;
            std::cout << "\n\nStack #" << i << "\n";
            PrintVectors::printVector(*it);
        }
    }

    void printMultipleVectorsMinimal(std::vector<std::vector<card>> vecs)
    {
        int i=1;
        for (auto it = begin(vecs); it != end(vecs); ++it)
        {
            if(i<9)
                std::cout << "#" << i << ": ";
            if(i==9)
                std::cout << "#9-H: ";
            if(i==10)
                std::cout << "#10-D: ";
            if(i==11)
                std::cout << "#11-S: ";
            if(i==12)
                std::cout << "#12-C: ";
            PrintVectors::printVector(*it);
            std::cout << "\n";
            ++i;
        }
    }
};

class InputDriver
{
    int input;
    public:

    InputDriver(){
        input = validInputCheck();
        input = validStack(input);
    }
    InputDriver(std::vector<card> stack){
        input = validInputCheck();
        input = validCardPosition(stack);
    }

    int returnInput(){
        return input;
    }
    
    int validInputCheck(){
        while(true)
        {
            if(std::cin >> input)
            {
                return input;
            }
            if(std::cin.fail())
            {
                std::cout << "NOT AN INTEGER, REENTER:";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    int validStack(int input){
        while(input<0 || input>13)
        {
            std::cout << "NOT A VALID STACK #, REENTER:";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            input = validInputCheck();
        }
        return input;
    }
    int validCardPosition(std::vector<card> stack){
        while(input<0 || input>stack.size())
        {
            std::cout << "NOT A VALID CARD POSITION, REENTER:";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            input = validInputCheck();
        }
        return input;
    }
};



std::vector<card> fillCardDeck(std::vector<card> cardDeck)
{
    int i = 1, j = 1;
    for (i; i < 5; ++i)
    {
        for (j; j < 14; ++j)
        {
            if (i == 1)
                cardDeck.push_back({"C", j, false});
            if (i == 2)
                cardDeck.push_back({"H", j, false});
            if (i == 3)
                cardDeck.push_back({"S", j, false});
            if (i == 4)
                cardDeck.push_back({"D", j, false});
        }
        j = 1;
    }
    return cardDeck;
}

std::vector<card> shuffleCardDeck(std::vector<card> cardDeck)
{
    std::random_device rd; // uniformly-distributed integer rng, producing n-d random numbers

    std::mt19937 g(rd()); // mersenne twister engine pseudorandom number generator
    shuffle(cardDeck.begin(), cardDeck.end(), g);
    return cardDeck;
}

std::vector<std::vector<card>> cardMove(std::vector<std::vector<card>> stacks, int fromCardIndex)
{  
    card temporaryCard;
    const int frontStackSize = stacks.front().size();
    card *fromCardIndexPointer = &stacks.front().at(fromCardIndex-1); //fromCardIndexPointer points to vector index from which cards are swapped and erased
    for(int i=fromCardIndex;i<=frontStackSize;++i){ //Cards are swapped and erased with each iteration
        temporaryCard = *fromCardIndexPointer;
        stacks.front().erase(stacks.front().begin()+fromCardIndex-1);
        stacks.back().push_back(temporaryCard);
    }
    
    if(stacks.front().size()!=0)
        stacks.front().back().visible = true;
    return stacks;
}

int kingRule(std::vector<card> toStack, bool isHomeStack)
{
    if (isHomeStack == false){  //Check if moving to a stack on the board, and if that stack is empty
        if (toStack.size() == 0)
        {
            return 1;
        }
        return 0;
    }
    else if(toStack.size()!=0){ //Check for invalid move to empty homestack to avoid segfault on next line
        if (toStack.back().number == 12)
        {
            return 1;
        }
    }
    return 0;
}

int emptyRule(std::vector<card> toStack)
{
    if (toStack.size()==0)
    {
        return 1;
    }else{
        return 0;
    }
}

int aceRule(std::vector<card> toStack, bool isHomeStack)
{
    if (isHomeStack == false){  //Check if moving to a stack on the board
        if(toStack.size()!=0)   //Check if stack being moved to is empty, to avoid segfault on next if statement
        {
            if (toStack.back().number == 2)
            {
                return 1;
            }
        }
        return 0;
    }
    if (toStack.size() == 0)
    {
        return 1;
    }
    return 0;
}

int suitRule(card fromCard, card toCard, bool isHomeStack){
    if (isHomeStack==true){
        if (fromCard.suit==toCard.suit)
        {
            return 1;
        }
        return 0;
    }
    else
    {
        if ((fromCard.suit == "H" || fromCard.suit == "D") && (toCard.suit == "C" || toCard.suit == "S"))
        {
            return 1;
        }
        else if ((fromCard.suit == "C" || fromCard.suit == "S") && (toCard.suit == "H" || toCard.suit == "D"))
        {
            return 1;
        }
    }
    return 0;
}

int numberRule(card fromCard, card toCard, bool isHomeStack){
    if (isHomeStack == true){
        if(fromCard.number== toCard.number + 1)
        {
            return 1;
        }
        return 0;
    }
    if (fromCard.number == toCard.number - 1)
    {
        return 1;
    }
    return 0;
}

std::vector<std::vector<card>> cardCheck(std::vector<std::vector<card>> stacks, int toStackIndex, int fromCardIndex)
{
    card fromCard, toCard;
    std::vector<card> fromStack = stacks.front();
    std::vector<card> toStack = stacks.back();
    int validMove=0;
    bool isHomeStack=false;
    
    //First check for fromStack not being empty. If it is, error & break
    //Check for multiple cards being moved: if fromCardIndex is not the index of the final card on fromStack
    if(fromStack.size()!=0)
    {
        if(fromCardIndex==stacks.front().size()){
            fromCard = fromStack.back();    
        }
        else{
            fromCard = fromStack.at(fromCardIndex-1);
        }
    }
    else
    {
        std::cout << "\n\nINVALID MOVE:EMPTY STACK\n\n";
        return stacks;
    }

    //Check if fromCard is moving to its suits respective homestack
    switch(toStackIndex){
        case 8:
        {
            if (fromCard.suit == "H")
                isHomeStack=true;
        }
        break;
        case 9:
        {
            if (fromCard.suit == "D")
                isHomeStack=true;
        }
        break;
        case 10:
        {
            if (fromCard.suit == "S")
                isHomeStack=true;
        }
        break;
        case 11:
        {
            if (fromCard.suit == "C")
                isHomeStack=true;
        }
        break;
        default:
        break;
    }

    //Cannot move to foundation stack(Stack 8)
    if (toStackIndex==7){
        return stacks;
    }
    //Layer 1
    if (fromCard.number == 13){
        validMove += kingRule(toStack, isHomeStack);
    }
    if (fromCard.number == 1){
        validMove += aceRule(toStack, isHomeStack);
    }
    if (validMove>0){
        stacks = cardMove(stacks,fromCardIndex);
        return stacks;
    }
    //Layer 2
    //Can now fill toCard, as for a valid move there must be at least 1 card in the toStack from which toCard is derived
    if (toStack.size()!=0)
    {
        toCard = toStack.back();
    }else{
        std::cout << "\n\nINVALID MOVE\n\n";
        return stacks;
    }
    validMove += suitRule(fromCard, toCard, isHomeStack);
    validMove += numberRule(fromCard, toCard, isHomeStack);
    
    if (validMove>1){
        stacks = cardMove(stacks,fromCardIndex);
        return stacks;
    }
    return stacks;
}

std::vector<std::vector<card>> boardConstructor(std::vector<card> vec){
    std::vector<std::vector<card>> allStacks;
    int rangePointer=0;
    int startRange, endRange;
    for (int i=0;i<7;++i){
        startRange=rangePointer;
        rangePointer+=(i+1);
        endRange=rangePointer;
        std::vector<card> stackConstructor(vec.rbegin()+startRange, vec.rbegin()+endRange);
        allStacks.push_back(stackConstructor);
    }

    std::vector<card> stack8(vec.rbegin()+28, vec.rend());
    allStacks.push_back(stack8);
    std::vector<card> homeStack;
    for (int i=0;i<4;++i){
        allStacks.push_back(homeStack);
    }

    for (int i = 0; i < allStacks.size(); ++i)
    {
        if (allStacks[i].size() != 0)
            allStacks[i].back().visible = true;
    }
    return allStacks;
}

bool checkWin(std::vector<std::vector<card>> allStacks)
{
    int check=0;
    for(int i=0;i<8;++i){
        if(allStacks[i].size()==0)
        {
            ++check;
        }
    }
    if(check==7)
    {
        return true;
    }
    return false;
} 
int main()
{
    PrintVectors printVectors;
    std::vector<card> cardDeck;

    cardDeck = fillCardDeck(cardDeck);
    cardDeck = shuffleCardDeck(cardDeck);

    std::vector<std::vector<card>> allStacks = boardConstructor(cardDeck);

    printVectors.printMultipleVectorsMinimal(allStacks);

    card temporaryCard;
    int input=1;

    while (input != 0)
    {
        bool exit = checkWin(allStacks);
        if(exit==true){
            std::cout << "\nFINISH\n";
            break;
        }
        std::cout << "Enter stack #\nEnter 13 to cycle foundation\nEnter 0 to exit:";
        InputDriver inputFromStackNum;
        input = inputFromStackNum.returnInput();
        if (input == 13)
        {
            temporaryCard.suit = allStacks[7].back().suit;
            temporaryCard.number = allStacks[7].back().number;
            temporaryCard.visible = false;

            allStacks[7].pop_back();
            allStacks[7].insert(allStacks[7].begin(), temporaryCard);
            allStacks[7].back().visible = true;
        }
        else if (input == 0)
        {
            break;
        }
        else
        {

            int fromStackIndex = input;
            int toStackIndex;
            int fromStackCardIndex;
            int i=0;

            for(auto it:allStacks[fromStackIndex-1]){
                if (it.visible == true)
                    ++i;
            }
            if (i>1){
                std::cout << "\nEnter card position from left: ";
                InputDriver inputCardNum(allStacks[fromStackIndex-1]);
                fromStackCardIndex = inputCardNum.returnInput(); 
            }
            else{
                fromStackCardIndex = allStacks[fromStackIndex-1].size();
            }

            std::cout << "\nEnter stack to move to: ";
            InputDriver inputToStackNum;
            toStackIndex = inputToStackNum.returnInput();
            std::vector<std::vector<card>> stacks;
            stacks.insert(stacks.end(), {allStacks[fromStackIndex - 1], allStacks[toStackIndex - 1]});
            stacks = cardCheck(stacks, toStackIndex-1, fromStackCardIndex);
            allStacks[fromStackIndex - 1] = stacks.front();
            allStacks[toStackIndex - 1] = stacks.back();
        }
        printVectors.printMultipleVectorsMinimal(allStacks);
    }

    return 0;
}