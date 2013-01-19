#!/usr/bin/env python

from lxml import etree, html
from urllib import urlencode,urlretrieve
from urllib2 import Request, urlopen
from os import chdir

from pymongo import Connection
con = Connection('localhost')
db  = con['wimb']

bikes = db['bikes']

CRAIG_ADDR = "http://www.craigslist.org/about/sites"
craig_response = urlopen(CRAIG_ADDR)
craig_root = html.parse(craig_response)

us = craig_root.xpath('//div[@class="colmask" and h1/a[@name="US"]]')
if us != []:
    cities = us[0].xpath('.//li/a/@href')
    for city in cities:
        addr = city + "/bia"
        city_response = urlopen(addr)
        city_root = html.parse(city_response)
        rows = city_root.xpath('//p[@class="row"]')

        for row in rows:

            url = row.xpath('a/@href')[0]
            postId = url.split('/')[-1].split('.')[0]
            
            if bikes.find_one({'postID' : postId}):
                break

            print "Parsing..." + postId

            row_response = urlopen(url)
            row_root = html.parse(row_response)

            bodyL = row_root.xpath('//section[@id="postingbody"]')
            if bodyL != []:
                body = bodyL[0].text
            else:
                body = ""

            imgUrl = row_root.xpath('//div[@id="thumbs"]/a/@href')

            dateL = row_root.xpath('//date')
            if dateL != []:
                date = dateL[0].text
            else: 
                date = ""

            print postId
            print date
            print imgUrl
            print body
            bike = { 'postID' : postId,
                     'date'   : date,
                     'imgs'   : imgUrl,
                     'body'   : body 
                   }
            
            bikes.insert(bike)
