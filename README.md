# 컴파일

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

`src/threadCommon.hpp` 파일에 NUMCPUS 수치 변경 후 다시 컴파일

# 실행 환경

## CPU : i7-8750H(6코어 12쓰레드)