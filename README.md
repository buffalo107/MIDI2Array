MIDIファイルから配列を生成するWindows用アプリケーションです。

以下説明  

------------------------------------------------------------------------------------------------------------

##フォルダ構成

Visual Studio　->　ソースファイルとか。  
MIDI2Array.exe　->　ソフト本体。ダウンロードして起動すれば動く（はず）  
README.md　->　これ

※2023年12月16日追記：ソースファイルをビルドする時はソリューションプラットフォームをx86にしてください。  

------------------------------------------------------------------------------------------------------------

##使い方

複雑なソフトじゃないので使い方はすぐわかると思います。

1.ソフトを起動する  
2.白い枠にMIDIファイルをドラッグ＆ドロップ、もしくはファイル選択ボタンを押して、変換するMIDIファイルを選ぶ  
3.出力先のフォルダを選ぶ  
4.変換ボタンを押す  
5.配列が記述されたテキストファイルが出力先のフォルダに生成される  
6.クリアボタンを押すとファイルの選択状態が初期化されます（フォルダの選択はそのまま）  

変換元のMIDIファイルは、なるべく１トラック中の同時発音数が１音のみ、つまり音が重ならないように作成してください。  
また、変換後の配列はint型で、  
`｛ノートナンバー（音階）, ベロシティー（音の大きさ）, 音を鳴らす長さ（ミリ秒）,......｝`  
のフォーマットで出力されます。  
配列の型は使用環境に合わせて適宜変えてください。

※2023年12月16日追記：  
音量データ（ベロシティー）を出力ファイルの配列に含めるかどうかのチェックボックスを追加しました。  
このチェックボックスを選択しない場合、出力ファイルの配列のフォーマットは  
`｛ノートナンバー（音階）,音を鳴らす長さ（ミリ秒）,......｝`  
になります。チェックボックスはデフォルトで未選択になっています。

なお、MIDIの規格ではノートナンバーは0\~127と決められている（0\~127のナンバーに対して周波数が1対1で割り振られている）のですが、このアプリ（MIDI2Array）では無音時のノートナンバーとして128を追加で出力するようにしています。  

------------------------------------------------------------------------------------------------------------

##使用される方へのお願い

初めて作ったソフトなので、バグや拙い実装の部分が多々あると思います。  
一応デバッグはしましたが、責任は持てないので使用に関しては自己責任でお願いします。

あと改善点やバグがあったら教えてくださると助かります。  
[ツイッター](https://twitter.com/kukiwakame107)のリプとかDMとかでも結構です。よろしくお願いします。

------------------------------------------------------------------------------------------------------------

##謝辞

本ソフトの作成に当たっては、[こちら](https://midifile.sapp.org/)のライブラリを使用させていただきました。

[ライブラリ製作者の方のGitHub](https://github.com/craigsapp)も載せておきます。

C++で書かれたMIDIパーサーはなかなか無かったのでとても助かりました。  
素人の僕にもわかるほど完成度が高く、また素人の僕でも使えるほど使いやすかったです。多大なる感謝を申し上げます。  
また、ライブラリの使用やソフトの公開について、何か問題がありましたらすぐに取り下げますのでご連絡ください。

------------------------------------------------------------------------------------------------------------

##駄文

ソフトの作り方とかなんもわからんくてライブラリのフォルダごとプロジェクトにぶち込んでGitHubに上げてるんだけどいいのかな……。  
でもこれないとビルドできないし……。  
一応ライセンスに再配布してもいいよって書かれてたから大丈夫なはず……。  
GitHubなんもわからんわ……C++もわからん……なんならこのREADMEの書き方もよくわかってない。これでいいのかな？だめ？

なにもわからない。

------------------------------------------------------------------------------------------------------------

以上、よろしくお願いします。

------------------------------------------------------------------------------------------------------------

##おまけ：ノートナンバーと周波数の対応配列
```
float Note_to_Freq[]={  
    8.2 , 8.7 , 9.2 , 9.7 , 10.3 , 10.9 , 11.6 , 12.2, 13.0 , 13.8 ,  
    14.6 , 15.4 , 16.4 , 17.3 , 18.4 , 19.4 , 20.6 , 21.8 , 23.1 , 24.5 ,  
    26.0 , 27.5 , 29.1 , 30.9 , 32.7 , 34.6 , 36.7 , 38.9 , 41.2 , 43.7 ,  
    46.2 , 49.0 , 51.9 , 55.0 , 58.3 , 61.7 , 65.4 , 69.3 , 73.4 , 77.8 ,  
    82.4 , 87.3 , 92.5 , 98.0 , 103.8 , 110.0 , 116.5 , 123.5 , 130.8 , 138.6 ,  
    146.8 , 155.6 , 164.8 , 174.6 , 185.0 , 196.0 , 207.7 , 220.0 , 233.1 , 246.9 ,  
    261.6 , 277.2 , 293.7 , 311.1 , 329.6 , 349.2 , 370.0 , 392.0 , 415.3 , 440.0 ,  
    466.2 , 493.9 , 523.3 , 554.4 , 587.3 , 622.3 , 659.3 , 698.5 , 740.0 , 784.0 ,  
    830.6 , 880.0 , 932.3 , 987.8 , 1046.5 , 1108.7 , 1174.7 , 1244.5 , 1318.5 , 1396.9 ,  
    1480.0 , 1568.0 , 1661.2 , 1760.0 , 1864.7 , 1975.5 , 2093.0 , 2217.5 , 2349.3 , 2489.0 ,  
    2637.0 , 2793.8 , 2960.0 , 3136.0 , 3322.4 , 3520.0 , 3729.3 , 3951.1 , 4186.0 , 4434.9 ,  
    4698.6 , 4978.0 , 5274.0 , 5587.7 , 5919.9 , 6271.9 , 6644.9 , 7040.0 , 7458.6 , 7902.1 ,  
    8372.0 , 8869.8 , 9397.3 , 9956.1 , 10548.1 , 11175.3 , 11839.8 , 12543.9	, 0.0  
};
```

`Note_to_Freq[ノートナンバー]`とすると、そのナンバーに対応する周波数の値が得られます。