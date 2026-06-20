# embedded-learning

PICマイコンを中心に、PWM / UART / I2C / RS485 / DMX512 などの組み込み制御を学習・検証しているリポジトリです。

照明制御の現場経験をもとに、実機で試した内容を段階的に記録しています。

## 学習テーマ

- PWM制御
- UART通信
- I2C LCD表示
- 14pin パラレルLCD表示
- RS485通信
- DMX512受信
- PWM出力
- Raspberry Pi + QLC+ によるDMX制御

## 使用している主な環境

- MCU: PIC16F1938 / PIC16F1939
- Compiler: XC8
- IDE: MPLAB X IDE
- Writer: PICkit 5
- Measurement: Oscilloscope
- Other: Raspberry Pi / QLC+

## 学習の流れ

### 1. PWM制御

PICのタイマとCCP機能を使って、LEDの明るさ制御を学習しました。

- ハードウェアPWM
- ソフトウェアPWM
- デューティ比の変更
- EEPROM保存
- ボタン操作による設定変更

### 2. UART通信

PICとPC間でシリアル通信を行い、送受信の基本を確認しました。

- 9600bps通信
- 文字送受信
- リングバッファ
- OERR / FERR 対策
- コマンドによる動作切替

### 3. I2C LCD表示

I2C接続のキャラクタLCDに文字を表示しました。

- I2C通信
- LCD初期化
- 文字列表示
- 表示位置制御

### 4. 14pin パラレルLCD表示

一般的なキャラクタLCDをパラレル接続で制御しました。

- 8bitモード
- 4bitモード
- Busy Flag確認
- コマンド送信
- データ送信

### 5. RS485通信

UART通信をRS485トランシーバで差動通信化しました。

- MAX485 / ADM1485
- DE / RE 制御
- 終端抵抗
- 差動通信
- 送受信切替

### 6. DMX512受信

RS485通信を応用し、DMX512信号の受信とPWM出力を目標にしています。

- 250kbps通信
- Break検出
- Start Code確認
- チャンネルデータ受信
- 受信値によるPWM制御

## 関連ブログ

学習内容はブログにもまとめています。

- 組み込み制御ラボ  
  https://embedded-control-lab.com/

## 目的

照明制御の現場経験を活かしながら、マイコン・通信・DMX制御を自分で設計・実装できる技術者を目指しています。