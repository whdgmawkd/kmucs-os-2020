# 컴파일

<b>소스코드를 수정한 경우 `make clean` 없이 `make`하여도 자동으로 의존성을 파악하여 새롭게 컴파일 진행. 단, `DEBUG=y`를 사용할 경우 `make clean` 작업이 필요</b>

## 기본값

```
make
```

## 쓰레드 실행 정보 포함

```
make DEBUG=y
```

# 실행

```
make run
```

# 초기화

```
make clean
```

# 쓰레드 수 변경

`src/threadCommon.hpp` 파일의 <b>`NUMCPUS`</b> 수치 변경 후 다시 컴파일

# 출력 파일

## `figure295.csv`

Figure 29.5 그래프를 그리는 데 필요한 정보가 들어있는 CSV파일

### CSV 정보

| 헤더        | 내용                                                       |
|-------------|------------------------------------------------------------|
| type        | LockCounter와 ApproximateCounter                           |
| threadCount | 실행하는데 사용한 쓰레드 수                                |
| insertCount | 모든 쓰레드가 증가시킨 카운터 총합 (1000000 * threadCount) |
| time        | 실행하는데 소요된 시간                                     |
| threshold   | ApproximateCounter에서 사용한 S값 (LockCounter의 경우 0)   |

## `figure296.csv`

Figure 29.6 그래프를 그리는 데 필요한 정보가 들어있는 CSV파일

### CSV 정보

| 헤더        | 내용                                                       |
|-------------|------------------------------------------------------------|
| type        | ApproximateCounter                                         |
| threadCount | 실행하는데 사용한 쓰레드 수                                |
| insertCount | 모든 쓰레드가 증가시킨 카운터 총합 (1000000 * threadCount) |
| time        | 실행하는데 소요된 시간                                     |
| threshold   | 실행하는데 사용한 S값                                      |

## `figure2911.csv`

Figure 29.11 그래프를 그리는 데 필요한 정보가 들어있는 CSV파일

### CSV 정보

| 헤더        | 내용                                                                   |
|-------------|------------------------------------------------------------------------|
| type        | ConcurrentHashTable, ConcurrentList, ConcurrentQueue, LockCoupling     |
| threadCount | 실행하는데 사용한 쓰레드 수                                            |
| insertCount | 각 쓰레드가 insert한 개수 (Figure 29.5, Figure 29.6의 CSV 형식과 다름) |
| time        | 실행하는데 소요된 시간                                                 |

# 폴더 및 파일 구조

## `src` 폴더

소스코드(*.cpp, *.hpp)가 들어있는 폴더

### `main.cpp`

main 함수가 위치한 파일

### `main.cpp` 이외의 파일

각종 Lock-based Data Structure에 대한 자료 구조 및 함수가 선언/구현 되어있는 헤더파일 및 코드

## `build` 폴더

컴파일 결과물(*.o, *.d(증분 빌드를 위한 헤더 의존성 파일), a.out)이 생성되는 폴더

## `Makefile`

`make` 명령을 사용하여 간편하게 컴파일 및 실행 할 수 있도록 미리 구성된 스크립트

## `README.md`

각종 설명이 적혀있는 파일

