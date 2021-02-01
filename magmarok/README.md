# Magmarok

## intro
* Magmarok is a boss (flag) at Molten Cave.
* snowball can damage him a lot, but when his HP is less than half, he will heal himself to full.
* others weapon damage it little.
* maybe he can't be defeat on general way.

## walkthrough
1. reverse its gamelogic by ida
    * at Magmarok::GetMaxHealth can get its MaxHP is 10000
    * at Magmarok::Damage can get sth important
        1. fireball will heal its HP by get negative damage, if heal > 10000 - m_health, heal = 10000 - m_health, and damage = heal * -1, but heal is unsigned, so if m_health > 10000 once, it can be heal to intetger overflow and dead
        2. use snowball attack, damage will be *4
        3. others only damage half
    * at Magmarok::Tick can found if 0 < his HP < 5000, he will do healing and its HP + 4975, it won't check its HP is over 10000
    according to above, can use snowball attack him until its HP less than 5000 and then use fireball heal his HP over 5025, then its HP will over 10000, we use fireball heal him until its HP overflow to negative, and he will die.
    **need to know its HP**

2. build proxy
    for know its HP, can build a proxy to analysis its TCP packet.
    LiveOverflow do it at his YT and the proxy code is open source, so I just use his proxy and modify.
    it just a socket link to game client and game server, so need to change dns let game server's domain IP be the proxy's IP (localhost) and let proxy link to game server's IP.

3. analysis TCP packet
    * first 2 bytes is packet id
    * when sth damage, server will send a packet with packet_id: 0x2b2b, obj_id: int (like unsigned), HP: signed int and sth I don't know, can parse it to get HP

Then just attack like above can success.
I try to let it attack automatic, I let it get health packet then throw fireball or iceball, but Magmarok's fireball can cancel my fireball and iceball, so it will be interrupt.