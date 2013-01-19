#!/usr/bin/env python

from lxml import etree, html
from urllib2 import Request, urlopen
import csv

def loadCraig():
    craigDict = dict()
    with open('craig.csv') as craig:
        craigKV = csv.reader(craig)
        for row in craigKV:
            craigDict[row[0]] = row[1]
    return craigDict

def storeCraig(craigDict):
    with open('craig.csv', 'w') as craig:
        craigWriter = csv.writer(craig)
        for k, v in craigDict.iteritems():
            craigWriter.writerow([k,v])
    return

craigDict = loadCraig()

CRAIG_ADDR = "http://www.craigslist.org/about/sites"
craig_response = urlopen(CRAIG_ADDR)
craig_root = html.parse(craig_response)

us = craig_root.xpath('//div[@class="colmask" and h1/a[@name="US"]]')
if us != []:
    cities = us[0].xpath('.//li/a/@href')
    for city in cities:
        
        try:
            lastForCity = craigDict[city]
        except:
            lastForCity = ""
        
        addr = city + "/bia"
        city_response = urlopen(addr)
        city_root = html.parse(city_response)
        rows = city_root.xpath('//p[@class="row"]')
        if rows != []:
            url = rows[0].xpath('a/@href')[0]
            newLast = url.split('/')[-1].split('.')[0]
        else:
            newLast = lastForCity

        for row in rows:

            url = row.xpath('a/@href')[0]
            postId = url.split('/')[-1].split('.')[0]
            
            if postId == lastForCity:
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
        
        craigDict[city] = newLast
    
storeCraig(craigDict)
