<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY % html "IGNORE">
<![%html;[
<!ENTITY % print "IGNORE">
<!ENTITY docbook.dsl PUBLIC "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN" CDATA dsssl>
]]>
<!ENTITY % print "INCLUDE">
<![%print;[
<!ENTITY docbook.dsl PUBLIC "-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN" CDATA dsssl>
]]>
]>
<!-- Put the MW customizations here  -->

<style-sheet>
<style-specification id="print" use="docbook">
<style-specification-body> 

;; ====================
;; customize the print stylesheet
;; ====================

(define %left-margin% 
  4pi)

(define %right-margin% 
  4pi)

(define %graphic-extensions%
  '("jpg" "png" "tex" "gif"))
(define %graphic-default-extension%
  "tex")
(define preferred-mediaobject-notations
  (list "TEX" "JPG" "JPEG" "PNG" "linespecific"))
(define preferred-mediaobject-extensions
  (list "tex" "jpg" "jpeg" "png"))

</style-specification-body>
</style-specification>

<!--
;; ====================
;; customize the html stylesheet
;; ====================
-->
<style-specification id="html" use="docbook">
<style-specification-body>

(define %graphic-default-extension%
  "png")
(define %graphic-extensions%
  '("jpg" "png" "tex" "eps" "pdf"))
(define preferred-mediaobject-notations
  (list "JPG" "PNG" "linespecific"))
</style-specification-body>
</style-specification>

<external-specification id="docbook" document="docbook.dsl">
</style-sheet>

