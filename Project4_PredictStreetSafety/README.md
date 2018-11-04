<b>#4. 거리안전도 예측</b> <br>
- 개발인원 : 1명 <br>
- 개발기간 : 2017.01 ~ 2018.01 <br>
- 개발환경 : Windows <br>
- 개발언어 : C++, Python <br>
- 개발 툴  : Microsoft Visual Studio 2013, JetBrains PyCharm Community Edition 2017 <br>
- 작품소개 : 딥러닝을 통해 주어진 영상의 Context를 추출하여 거리 안전도 점수를 예측하는 모델. <br>
- 작품내용 : 거리 영상에서 눈에 띄는 부분을 찾아 Context를 추출하였고, 이를 활용해서 거리 안전도 점수를 예측하는 모델을 학습시켰다. <br>
- 개발 내용 <br>
_1. Pairwise Comparison을 진행하기 위한 프로그램 구현 <br>
_2. Convolutional Auto-Encoder를 이용한 Saliency Map을 구해서 Context영역 추출 <br>
_3. TrueSkill알고리즘을 통해 각 영상에 대한 랭킹을 매김. 순위를 바탕으로 0~10점으로 Normalization함. <br>
_4. Context 데이터를 입력, 랭킹 점수를 예측 Target으로 학습시켰음. <br>
(Tensorflow 사용) <br>
