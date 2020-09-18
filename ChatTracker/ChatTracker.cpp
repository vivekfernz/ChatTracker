// ChatTracker.cpp



#include "ChatTracker.h"
#include <string>
#include <vector>
#include <functional>
using namespace std;


//Keeps track of the relevant info
struct Info
{
    Info(string u, string c) : user(u), chat(c), count(0) {}
    int count;
    string user;
    string chat;
};


class ChatTrackerImpl
{
public:
    ChatTrackerImpl(int maxBuckets);
    ~ChatTrackerImpl();
    int dohash(string a);
    void join(string user, string chat);
    int terminate(string chat);
    int contribute(string user);
    int leave(string user, string chat);
    int leave(string user);

private:
    int bucketCount;
    //two hash tables
    vector<vector<Info>> table;
    vector<vector<Info>> chatsleft;

};

ChatTrackerImpl::ChatTrackerImpl(int maxBuckets)
{
    bucketCount = maxBuckets / 2;
    table.resize(bucketCount);
    chatsleft.resize(bucketCount);
}

ChatTrackerImpl::~ChatTrackerImpl()
{//does nothing because default destructor is enough
}

int ChatTrackerImpl::dohash(string a)
{//create key
    hash<string> str_hash;
    unsigned int hashValue = str_hash(a);
    unsigned int bucketNum = hashValue % bucketCount;
    return bucketNum;
}
void ChatTrackerImpl::join(string user, string chat)
{
    int bucketNum = dohash(user);

    vector<Info> ::iterator it = table[bucketNum].end();

    while (it > table[bucketNum].begin())
    {
        it--;
        //find existing info and replace
        if (it->chat == chat && it->user == user)
        {
            Info temp = *it;
            table[bucketNum].erase(it);
            table[bucketNum].push_back(temp);
            return;
        }
    }
    table[bucketNum].push_back(Info(user, chat));
}

int ChatTrackerImpl::terminate(string chat)
{
    int t_total = 0;
    int i = 0;
    for (i; i < bucketCount; i++)
    {
        if (!table[i].empty())
        {
            vector<Info>::iterator it = table[i].begin();
            while (it != table[i].end())
            {
                //find every person in the chat currently
                if (it->chat == chat)
                {
                    t_total += it->count;
                    it = table[i].erase(it);
                }
                else
                    it++;
            }
        }
    }
    int chatNum = dohash(chat);
    vector <Info> ::iterator itt = chatsleft[chatNum].begin();
    while (itt != chatsleft[chatNum].end())
    {
        if (itt->chat == chat)
        {
            //find everybody who left the chat
            t_total += itt->count;
            itt = chatsleft[chatNum].erase(itt);
        }
        else
            itt++;
    }
    return t_total;
}

int ChatTrackerImpl::contribute(string user)
{
    int bucketNum = dohash(user);
    vector<Info>::iterator it = table[bucketNum].end();
    while (it > table[bucketNum].begin())
    {
        it--;
        if (it->user == user)
        {
            //find latest user chat
            it->count++;
            return it->count;
        }
    }
    return 0;


}

int ChatTrackerImpl::leave(string user, string chat)
{
    int bucketNum = dohash(user);
    vector<Info>::iterator it = table[bucketNum].end();
    while (it > table[bucketNum].begin())
    {
        it--;
        if (it->user == user && it->chat == chat)
        {
            //find user and chat
            int amount = it->count;
            int chatNum = dohash(chat);
            chatsleft[chatNum].push_back(*it);
            table[bucketNum].erase(it);
            return amount;
        }
    }
    return -1;

}

int ChatTrackerImpl::leave(string user)
{
    int bucketNum = dohash(user);
    vector<Info> ::iterator it = table[bucketNum].end();
    while (it > table[bucketNum].begin())
    {
        it--;
        if (it->user == user)
        {
            //find user latest chat
            int amount = it->count;
            int chatNum = dohash(it->chat);
            chatsleft[chatNum].push_back(*it);
            table[bucketNum].erase(it);
            return amount;
        }
    }

    return -1;

}

//*********** ChatTracker functions **************

// These functions simply delegate to ChatTrackerImpl's functions.
// You probably don't want to change any of this code.

ChatTracker::ChatTracker(int maxBuckets)
{
    m_impl = new ChatTrackerImpl(maxBuckets);
}

ChatTracker::~ChatTracker()
{
    delete m_impl;
}

void ChatTracker::join(string user, string chat)
{
    m_impl->join(user, chat);
}

int ChatTracker::terminate(string chat)
{
    return m_impl->terminate(chat);
}

int ChatTracker::contribute(string user)
{
    return m_impl->contribute(user);
}

int ChatTracker::leave(string user, string chat)
{
    return m_impl->leave(user, chat);
}

int ChatTracker::leave(string user)
{
    return m_impl->leave(user);
}
