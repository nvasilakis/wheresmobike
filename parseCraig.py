#!/usr/bin/env python

from lxml import etree, html
from urllib import urlencode,urlretrieve
from urllib2 import Request, urlopen
from os import chdir

DOWNPATH="vision/data/"
chdir(DOWNPATH)

CRAIG_ADDR = "http://www.craigslist.org/about/sites"
craig_response = urlopen(CRAIG_ADDR)
craig_root = html.parse(craig_response)

us = craig_root.xpath('//div[@class="colmask" and h1/a[@name="US"]]')
if us != []:
    cities = us[0].xpath('.//li/a/@href')
    for city in cities[:4]:
        addr = city + "/bia"
        city_response = urlopen(addr)
        city_root = html.parse(city_response)
        rows = city_root.xpath('//p[@class="row"]')

        for row in rows[:10]:

            url = row.xpath('a/@href')[0]
            row_response = urlopen(url)
            row_root = html.parse(row_response)
            body = row_root.xpath('//section[@id="postingbody"]')
#            imgUrl = row_root.xpath('//img[@id="iwi"]/@src') # Single Picture
            imgUrl = row_root.xpath('//div[@id="thumbs"]/a/@href')
            date = row_root.xpath('//date')

            print " -"
            print '   url: "' + url + '"'
            print '   color: '
            print '   type: '
            print '   make_or_model: '
            if imgUrl != []:
                print '   images: '
                for img in imgUrl:
                    name = img.split('/')[-1]
                    print '     - "' + name + '"'
                    urlretrieve(img, name)

