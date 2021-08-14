# Chromium 빌드 방법

크로미엄 공식문서에는 크로미엄을 다음과 같이 정의합니다.
> Chromium is an open-source browser project that aims to build a safer, faster, and more stable way for all users to experience the web.

우리가 잘 알고있는 구글 크롬은 이 크로미엄 코드를 사용하여 개발되었습니다. 이러한 크로미엄을 빌드하기 위한 과정을 알아보도록 하겠습니다.

[1. Install depot_tools](#Install-depot_tools)  
[2. Get the code](#Get-the-code)  
[3. Setting up the build](#Setting-up-the-build)  
[4. Build Chromium](#Build-Chromium)  
[5. Run Chromium](#Run-Chromium)  
[#. Update your checkout](#Update-your-checkout)  

크로미움의 소스파일은 방대합니다. 이런 대규모 프로젝트에 참여한다면 소스코드를 다운받거나, checkout, code reivew를 관리하는 것이 힘이 들 것입니다. 이러한 이유때문에 chromium source code Repository와 interaction하는 것을 용이하게 해주는 depot_tools를 이용하는 것입니다.
<br><br>
[1](#Install-depot_tools)  에서 depot_tools를 다운 받은 이후에 [2](#Get-the-code)에서 depot_tools에서 지원해주는 fetch도구를 이용해 chromium의 source code를 repository에서 다운받습니다. 
<br><br>
이제 다운 받은 소스코드를 빌드하여 실행파일로 만들어줘야합니다. 방대한 소스파일들을 빌드하기위해 빌드프로세스를 수동으로 호출하는 것이 실용적이지 않습니다. 무엇을 어떤 순서 빌드할지 또 어떤 의존성이 있는지 추적하기 쉽지 않기 때문입니다. 하지만 걱정할 필요가 없습니다. 우리는 앞서 depot_tools에 포함된 유틸리티 중 하나인 gn과 ninja 빌드도구를 사용하여 위의 복잡한 빌드과정을 자동으로 처리해줄 수 있습니다. ninja를 이용하여 빌드하기에 앞서 [3](#Setting-up-the-build) 에서 gn으로 빌드할 폴더를 만들어주고 환경변수를 설정합니다.
<br><br>
빌드할 환경을 모두 세팅한 이후에는 [4](#Build-Chromium)에서 ninja를 이용하여 크로미움을 빌드하고 빌드가 완료되면 [5](#Run-Chromium)를 참고하여 실행합니다.

빌드 이후에 크로미움 오픈소스가 업데이트가 되었다면 rebase를 한 이후에 depot_tools에 포함된 유틸리티 중 하나인 gclient를 이용하여 적절하게 동기화하여 코드를 쉽게 업데이트 할 수 있습니다.
<br>
<hr>  
  
### Install depot_tools  

```console
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
```
제일 먼저 [depot_tools](#depot_tools) repository를 clone합니다.
<br><br>
```console
export PATH="$PATH:/path/to/depot_tools"
```
이제 [환경변수](#environment-variable)를 설정해줍니다. 이로써 크로미움 오픈소스를 빌드할 빌드도구를 갖추게 되었습니다. 다음으로는 크롬의 소스코드를 다운받는 방법을 알아보겠습니다.
<br><br>
<hr>  
  
### Get the code

```console
git config --global core.precomposeUnicode true
```  
유니코드 파일 이름이 HFS에 의해 손상되지 않았는지 확인합니다.(파일 시스템이 파일 이름을 저장하는 방식이 다르기 때문에 발생할 수 있는 문제를 해결하는 방법으로 Mac과 다른 OS에서 저장소를 공유할 때 유용하게 사용되는 명령어라고 합니다.)
<br><br>
```console
mkdir chromium && cd chromium
```
체크아웃을 위한 chromium 디렉토리를 만들고 이 디렉토리로 접근합니다.
<br><br>
```console
fetch --nohooks chromium
```
depot_tools에 포함된 fetch 도구를 실행하여 chromium 소스코드를 다운받고 종속성을 확인합니다.

```console
cd src
```  
fetch가 완료되면 숨겨진 .gclient 파일과 work directory에 src 디렉토리가 생성됩니다. 이제부터 뒤에 이뤄질 과정은 src디렉토리에 접근했다는 것을 가정합니다. 이제 소스코드를 다운 받았으니 빌드도구로 이를 빌드할 일만 남았습니다. 
<br><br>
<hr>
  
### Setting up the build
```console
gn gen out/Default
```
gn을 이용하여 빌드를 위한 .ninja 파일을 생성합니다. 폴더 이름을 Default이외에 다른 것으로 바꿔도 되지만 out의 하위 디렉토리에 생성해야합니다.
<br><br>

다양한 build arguemnts를 설정할 수 있습니다. 이는 GN build configuration을 보며 필요한 arguements를 설정하시면 됩니다.
```console
gn gen out/Default
```
gn gen 명령어를 사용하여 vim으로 build arguments를 설정할 수 있습니다. 저는 "use_debug_fission=false", "is_clang=false"를 설정해주었습니다. 첫번째 argument는 링크시간을 희생시켜 GDB 로드 시간을 개선한다고 합니다.(물론 무슨 뜻인지 아직 이해가 가지 않습니다)
<br><br>
<hr>  
  
### Build Chromium
드디어 다운 받은 소스코드를 빌드할 때가 되었습니다.  
```console
autoninja -C out/Default chrome
```
ninja 대신 ninja에게 전달되는 인수에 대해 최적의 값을 자동으로 제공하는 wrapper인 autoninja를 이용하여 빌드를 진행합니다.

<br><br>
<hr>  
  
### Run Chromium  
빌드가 완료되면 chromium이 생성되었을 것입니다. chromium을 실행하는 명령어는 다음과 같습니다.
```console
out/Default/Chromium.app/Contents/MacOS/Chromium
```
<br><br>
<hr>  
  
### Update your checkout
기존의 checkout을 업데이트하기 위해서 다음 명령어를 사용하면 됩니다.
```console
$ git rebase-update
$ gclient sync
```
첫번째 명령어는 chromium source를 업데이트하고 두번째 명령어는 종속성을 적절한 버전과 동기화하고 필요에 따라 후크를 다시 실행합니다.(후크가 뭔지는 잘 모르겠습니다.)
<br><br>
<hr>  

#### environment variable
환경변수란 운영체제가 참조하는 변수를 말합니다. 여기서 변수는 우리가 실제로 프로그래밍할때 선언하는 그 변수를 의미합니다. 많은 변수들 중 $PATH변수에 대해 알아보겠습니다. ~/Documents 안에 a.out 프로그램이 존재한다고 가정합니다. a.out를 실행하기위해서 다음 명령어를 입력합니다.
```console
./a.out
``` 
하지만 현재 디렉토리에 a.out이 존재하지 않으면 실행되지않습니다. 이는 상황에 따라 매우 불편할 수 있으며 사용자가 어느 디렉토리에 있든 a.out을 실행시키고 싶을 수 있습니다. 따라서 편의를 위해서 $PATH라는 변수를 만들고 현재 디렉토리에 실행하고자하는 파일이 존재하지않는다면 $PATH변수에 등록된 디렉토리에 파일이 존재하는지 확인하고 이를 실행시키는 기능을 제공합니다.

export 명령은 환경변수를 설정하는 명령어이며 위와 같이 입력하면 $PATH에 ~/Documents 경로가 추가됩니다. 그런데 무언가 이상합니다. 
```console
export PATH="~/Documents"
```
이렇게 설정해줘야 할 것 같은데 디렉토리 경로 앞에 $PATH가 있다는걸 확인할 수 있습니다. $PATH는 기존 환경변수를 의미하는데 이를 빼고 바로 위와 같이 명령어를 작성하면 이전에 등록해놓았던 디렉토리들이 덮어쓰기가 됩니다. 그래서 다음과 같이 작성을 해주어야합니다.
```console
export PATH="$PATH:~/Documents"
```
이렇게 되면 우리가 어떤 디렉토리에 있던지 a.out을 실행하면 우리가 있는 디렉토리에 a.out이 존재하지않으면 $PATH에 저장된 디렉토리를 확인하여 a.out을 찾을 것이고 이를 실행합니다.
<hr>  
  
#### depot_tools
Chomium에서 사용하는 스크립트 패키지로 checkout, code reivew를 관리할 수 있습니다. 즉, Chromium source code Repository와 interaction을 가능하게 해줍니다. depot_tools에 포함된 유틸리티들(chromium 빌드 등에 필요한 도구들)은 다음과 같습니다.

* gclient
    * git checkout을 관리하기위한 meta-checkout tool
    * checkout은 특정 시점이나 브런치로 이동하는 것을 의미합니다.
* ninja
    * Chromium open source를 빌드하기위한 빌드도구
    * 빌드 도구란 소스코드에서 어플리케이션 생성을 자동화 하기위한 프로그램을 말합니다. 즉 종속성 다운로드, 소스코드를 바이너리코드로 컴파일, 바이너리 코드를 패키징 등의 소스코드 빌드를 위한 일련의 작업들을 자동으로 해주는 도구입니다.
* gn
    * ninja로 Chromium을 빌드 할 수 있도록 .Ninja 빌드 파일을 생성하는 메타 빌드 시스템
* git-cl
    * code review tool for git
* gcl
* git
* python
* repo

<hr>  
  
#### 참고자료하면 좋은 자료
Ninja에 대해 더 알고 싶을때: https://lethean.github.io/2016/03/17/about-ninja-build/  
<br>
#### 참고자료
빌드도구란? https://wangmin.tistory.com/50  
The Chromium Projects : https://chromium.googlesource.com/chromium/src/+/refs/heads/main/docs/mac_build_instructions.md#install
