# zh2py
汉字反查拼音的QT实现

本来想实现一个使用拼音首字母搜索树的功能，结果发现汉字反查拼音实现起来并没有那么容易，特别是多音字的处理。最初的想法是由多到少地厄取连续汉字去与输入法的词组做匹配的，这样操作的结果是输入法的词库很大，而厄取的连续汉字大部分不是词组，匹配成功的概率不高，因此查询很慢。幸好发现此处（ https://github.com/ZiQiangWang/zh_CN ） 使用Java Script很好地使用了此功能，效率不知高了多少倍数。
