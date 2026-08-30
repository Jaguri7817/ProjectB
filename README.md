# Project B


## 프로젝트 소개


Project B는 Unreal GAS 프레임워크를 탑재하여 Chronicle Gate의 전투 시스템을 개선하고, 재사용성과 확장성을 더해 유기적인 전투 메커니즘 구현을 목표로 하는 개인 프로젝트입니다.

기본 시스템 동작 설계는 C++로 구현하고, 세부 설정 및 데이터 설정은 Blueprint에서 설정하도록 구현했습니다.

## 개요


- 프로젝트 명: Project B
- 기간: 2026.07.~(진행 중)
- 진행 상태: GAS 기반 기본적인 전투 파이프라인 구축 (입력 → 콤보 공격 → 타겟 판정 → 데미지 처리 → 사망 처리)

## 기술 스택


- **Engine: 언리얼 엔진 5.6.1**
- **Language: C++**
- **Framework: GAS**(Gameplay Ability System)
- **Network: Dedicated Server, Replication**
- **Version Control: Git, GitHub**

## 구현 기능


### 1. GAS 기반 콤보 공격 시스템

- [GAS 기반 콤보 공격](https://github.com/Jaguri7817/ProjectB/blob/main/Source/ProjectB/GameplayAbility/PBGA_Attack.cpp#L60)

공격 애니메이션 몽타주의 **범용 Anim Notify**를 통해 ***Combo Check***와 ***Attack Hit*** 이벤트를 수신하면 다음 콤보 진행 및 공격 판정을 진행합니다.

### 2. 데이터 기반 Ability 입력 시스템

- [GAS 입력 시스템](https://github.com/Jaguri7817/ProjectB/tree/main/Source/ProjectB/Input)
- [GAS 입력 바인드](https://github.com/Jaguri7817/ProjectB/blob/main/Source/ProjectB/Character/PBCharacterPlayer.cpp#L115)

  입력을 통해 발동되는 Ability의 입력 데이터들을 구조체화하고 이를 데이터 에셋으로 관리하도록 설계했습니다.

### 3. Gameplay Event 기반 ‘애니메이션 ↔ 전투 시스템’ 신호 전달

- [범용 Notify](https://github.com/Jaguri7817/ProjectB/blob/main/Source/ProjectB/Animation/PBAnimNotify_SendGameplayEvent.cpp#L28)

  Notify가 호출되면 해당 Notify에 지정된 Tag를 담은 Gameplay Event를 전송해 Owner 액터의 ASC에게 신호를 전달합니다.

### 4. GAS 기반 공격 판정 및 데미지 처리 흐름

- [판정을 위한 타겟 액터](https://github.com/Jaguri7817/ProjectB/blob/main/Source/ProjectB/GameplayAbility/TargetActor/GATA_AttackHitCheck.cpp)

  공격자가 바라보는 방향으로 일정 크기의 Box를 생성하여 해당 Box에 오버랩되는 피해자가 있는지를 검출합니다.
  오브젝트 타입이 Pawn으로 설정된 컴포넌트 단위로 검출하기 때문에 캐릭터가 가지고 있는 캡슐 컴포넌트, 스켈레탈 메시 컴포넌트 등 중복 제거 뒤 타겟 데이터를 생성합니다.

- [데미지 처리 흐름](https://github.com/Jaguri7817/ProjectB/blob/main/Source/ProjectB/GameplayAbility/PBGA_Attack.cpp#L149)

   생성된 타겟 데이터를 가지고 데미지를 처리합니다.

<img width="400" height="217" alt="Image" src="https://github.com/user-attachments/assets/5e0492c4-4f25-4bde-a027-1aa5bef4d333" />
