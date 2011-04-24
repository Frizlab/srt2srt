# SubRib Subtitles Transformer #

## SubRib Subtitle (.srt) ##

SubRib Subtitle (.srt files) is one of the most widely used file
format to describe movie subtitles.

It is a text-base format which does not allow text-styling neither metadata
of the subtitles it describes. The timing of the subtitles is defined with an
elapsed time from the beginning of the movie.

Here is an example of a SubRip subtitle entry that will display the
text "Altocumulus clouds occur between six thousand" starting twenty seconds
after the beginning of the movie, and ending 4.4 seconds
after (from [Wikipedia][srt_article_wiki]):
```
1
00:00:20,000 --> 00:00:24,400
Altocumulus clouds occur between six thousand
<Blank line>
```

## Goal of the Project ##

With certain movies and certain SubRip files, synchronization problems
might appear. The srt2srt project is here to help modify the SubRib in
order to resynchronize them with their associated movie.

Supported functionalities:

* Currently none, the project just opened! Stay tuned for updates…

[srt_article_wiki]: http://en.wikipedia.org/wiki/SubRip
