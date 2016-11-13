from __future__ import unicode_literals
from random import randint
from django.db import models
# Create your models here.
class Team(models.Model):
	team_name = models.CharField("Team Name",unique = True,max_length=200)
	team_binary = models.IntegerField("Team LED Int",unique = True)
	def __unicode__(self):
		return u'{0}'.format(self.team_name)

class Challenges(models.Model):
	challenge_name = models.CharField("Challenge Name",unique = True,max_length=200)
	challenge_description = models.TextField("Challenge Description")
	challenge_pass = models.CharField("Challenge Password",unique = True,max_length=200)
	challenge_valid = models.CharField("Challenge Valid Code",default="complete",max_length=200)
	def __unicode__(self):
		return u'{0}'.format(self.challenge_name)

class Badge(models.Model):
	badge_id = models.CharField("Badge ID",unique = True,max_length=200)
	badge_challenges = models.ManyToManyField(Challenges, blank=True)
	badge_level = models.IntegerField("Badge Level")
	badge_nick = models.CharField("Badge Alias",unique = False,max_length=200,blank=True, null=True)
	badge_verify = models.CharField("Badge Verify Code",unique = False, null = True,max_length=200)
	badge_salt = models.CharField("Badge Salt",max_length=200)
	badge_team = models.ForeignKey(Team, db_index=True,related_name="team")
	badge_status = models.CharField("Badge Status",max_length=200,default="noob")
	def normaliseLevel(self):
		if(self.badge_level <= 0):
			self.badge_level = 1
		if(self.badge_level >= 6):
			self.badge_level = 5
	def __unicode__(self):
		return u'Badge [ id: %s , challenges: %s , status: %s, level: %s , nick: %s , salt: %s , team: %s ]' % (self.badge_id,self.badge_challenges,self.badge_status,self.badge_level,self.badge_nick,self.badge_salt,self.badge_team)

	'''
	@classmethod
	def create(cls, badgeID):
		badge = cls()
		badge.badge_id = badgeID
		badge.badge_challenges = []
		badge.badge_level = 1
		badge.badge_nick = None
		badge.badge_salt = "Andrew"
		badge.badge_team_id = randint(1,3)
        # do something with the book
		return badge
	'''
'''
	def __init__(self,badgeID):
		self.badge_id = badgeID
		self.badge_level = 1
		self.badge_nick = None
		self.badge_salt = "Andrew"
		self.badge_team_id = randint(1,3)
		#self.badge_team = Team.objects.get(pk=randint(1,3))
		#self.badge_challenges = None;
'''