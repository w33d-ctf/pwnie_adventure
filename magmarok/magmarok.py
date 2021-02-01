import struct

m_health = 10000

def get_m_health( data, port, origin ):
    packet_id = struct.unpack( ">H", data[0:2] )[0]
    if packet_id == 0x2b2b:
        id, health = struct.unpack( "Ii", data[0:8] )
        #print( "health: {} -> {}".format( id, health ) )
        if health > 4000:
            return health

def action():
    position1 = "6d76a3282e47cacbe84690c751444e075a0500000000".decode( 'hex' )
    skill = "2a691000477265617442616c6c734f66466972655e923e419cf15d41d639ea346d76a3282e47cacbe84690c751447808dd0900000000".decode( 'hex' )
    position2 = "6d76dc502d473d07ef469eed4144ee04b4e000000000".decode( 'hex' )
    return position1 + skill + position2

# def snowball():
#     # 6d76a3282e47cacbe84690c751444e075a0500000000
#     position1 = "6d76de6d4747fe9db1c59029b644b50aa07f00000000".decode( 'hex' )
#     skill = "2a6908005a65726f436f6f6ca7fb704100793343000000006d76de6d4747fe9db1c59029b644b50aa07f00000000".decode( 'hex' )
#     position2 = "6d7619454747059fa4c5d43abe441b033c8a00000000".decode( 'hex' )
#     return position1 + skill + position2

def fireball():
    # ZeroCool
    # spell_packet = "2a6908005a65726f436f6f6c0000b4c200000000839f1c40".decode('hex')

    # GreatBallsOfFire
    spell_packet = "2a691000477265617442616c6c734f66466972650000b4c200000000839f1c40".decode('hex')

    magmarok_position = struct.pack("=Hfff", 0x766d, 49692.42, -5662.38, 1840.12+800.0)+"00c00000f3890000".decode('hex')
    safe = struct.pack("=Hfff", 0x766d, 51031.42, -5212.42, 1508.91)+"00c00000f3890000".decode('hex')
    return magmarok_position + spell_packet + safe

def snowball():
    # ZeroCool
    spell_packet = "2a6908005a65726f436f6f6c0000b4c200000000839f1c40".decode('hex')

    # GreatBallsOfFire
    # spell_packet = "2a691000477265617442616c6c734f66466972650000b4c200000000839f1c40".decode('hex')

    magmarok_position = struct.pack("=Hfff", 0x766d, 49692.42, -5662.38, 1840.12+800.0)+"00c00000f3890000".decode('hex')
    safe = struct.pack("=Hfff", 0x766d, 51031.42, -5212.42, 1508.91)+"00c00000f3890000".decode('hex')
    return magmarok_position + spell_packet + safe