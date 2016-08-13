package com.aaomidi.jchat.chat.handlers;

import com.aaomidi.jchat.chat.Main;
import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.util.CharsetUtil;

public class ChatClientHandler extends ChannelInboundHandlerAdapter {
    private final Main instance;

    public ChatClientHandler(Main instance) {
        this.instance = instance;
    }


    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        System.out.printf("New client?\n");

    }


    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        ByteBuf message = (ByteBuf) msg;
        String m = message.toString(CharsetUtil.UTF_8);
        System.out.println(m);
        String[] array = m.split("\n");
        int[] pubKey = new int[2];
        if (array[0].startsWith("EHLO:3")) {
            pubKey[0] = Integer.valueOf(array[3].replace("\r", ""));
            pubKey[1] = Integer.valueOf(array[4].replace("\r", ""));
            instance.setOtherPublicKey(pubKey);
        }
        message.release();
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        System.err.printf("An exception was caught!\n");
        cause.printStackTrace();
        ctx.close();
    }
}
