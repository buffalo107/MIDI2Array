MIDIファイルから配列を生成するアプリです。

以下説明
//------------------------------------------------------------------------------------------------------------

Visual Studio　->　ソースファイルとか
MIDI2Array.exe　->　ソフト本体。これを起動すれば動く（はず）
README.md　->　これ

//------------------------------------------------------------------------------------------------------------

複雑なソフトじゃないので使い方はすぐわかると思います。

1.ソフトを起動する  
2.白い枠にMIDIファイルをドラッグ＆ドロップ、もしくはファイル選択ボタンを押して、変換するMIDIファイルを選ぶ  
3.出力先のフォルダを選ぶ  
4.変換ボタンを押す  
5.配列が記述されたテキストファイルが生成される
6.クリアボタンを押すとファイルの選択状態が初期化されます（フォルダの選択はそのまま）  

変換元のMIDIファイルは、なるべく１トラック中の同時発音数が１音のみ、つまり音が重ならないように作成してください。
また、変換後の配列はint型で、｛ノートナンバー（音階）, ベロシティー（音の大きさ）, 音を鳴らす長さ（ミリ秒）......｝のフォーマットで出力されます。

//------------------------------------------------------------------------------------------------------------

初めて作ったソフトなので、バグや拙い実装の部分が多々あると思います。
一応デバッグはしましたが、責任は持てないので使用に関しては自己責任でお願いします。

あと改善点やバグがあったら教えてくださると助かります。
[ツイッター](https://twitter.com/kukiwakame107)のリプとかDMとかでも結構です。よろしくお願いします。

//------------------------------------------------------------------------------------------------------------

本ソフトの作成に当たっては、[こちら](https://midifile.sapp.org/)のライブラリを使用させていただきました。

[ライブラリ製作者の方のGitHub](https://github.com/craigsapp)も載せておきます。

C++で書かれたMIDIパーサーはなかなか無かったのでとても助かりました。多大なる感謝を申し上げます。

また、ライブラリの使い方やソフトの公開について、何か問題がありましたらすぐに取り下げます。

//------------------------------------------------------------------------------------------------------------

ソフトの作り方とかなんもわからんくてライブラリのフォルダごとGitHubに上げてるんだけどいいのかな……。でもこれないとビルドできないし……。
一応ライセンスに再配布してもいいよって書かれてたから大丈夫なはず……。
GitHubなんもわからんわ……C++もわからん……なにもわからない。

//------------------------------------------------------------------------------------------------------------

以上、よろしくお願いします。