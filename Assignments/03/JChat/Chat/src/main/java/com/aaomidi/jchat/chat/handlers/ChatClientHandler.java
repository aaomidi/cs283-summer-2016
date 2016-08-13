package com.aaomidi.jchat.chat.handlers;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

public class ChatClientHandler extends ChannelInboundHandlerAdapter {
    int msgCount = 0;

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        System.out.printf("New client?\n");

    }


    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        ByteBuf message = (ByteBuf) msg;

        message.release();
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        System.err.printf("An exception was caught!\n");
        cause.printStackTrace();
        ctx.close();
    }
}
