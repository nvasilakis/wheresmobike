#!/usr/bin/env python

from lxml import etree, html
from urllib import urlencode,urlretrieve
from urllib2 import Request, urlopen
from os import chdir

url = "http://ec2-50-19-164-82.compute-1.amazonaws.com:8080/solr/core0/select?q=*&rows=4387&indent=on"

response = urlopen(url)
root = html.parse(response)

docs = root.xpath("//doc")
for doc in docs:
    urlNode = doc.xpath('str[@name="url"]')
    images  = doc.xpath('arr[@name="imageURL"]/str')
    if images != []:
        print " -"
        print "   url: '" + urlNode[0].text_content()
        print "   images:"
        for img in images:
            fn = img.text.split('/')[-1]
            urlretrieve(img.text, fn)
            print "    - '" + fn + "'"
