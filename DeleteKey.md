1. 라벨이 없으면 종이에서 라벨을 뗀다 this->label = Detach();

2.1. 현재 마지막 칸이 아니다 ` if(this->column < this->label->GetLength()) `
2.1.1. 현재 칸을 지우다 ` Erase(this->label->GetCurrent( ), 1); `

2.2. 현재 마지막 칸이고 마지막 줄이 아니면
2.2.1. 라벨에 문자열이 있으면 복사하고 삭제한다

2.2.2. 라벨에 문자열이 없으면 라벨을 삭제한다